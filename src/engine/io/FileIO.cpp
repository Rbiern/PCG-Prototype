#include "FileIO.hpp"

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <fstream>
#include <limits>
#include <system_error>
#include <utility>
#include <Windows.h>


namespace {
    std::string windowsError(DWORD code) {
        return std::error_code(static_cast<int>(code),std::system_category()).message();
    }


    struct TemporaryFile {
        std::filesystem::path path;
        HANDLE handle = INVALID_HANDLE_VALUE;

        TemporaryFile() = default;
        TemporaryFile(const TemporaryFile&) = delete;
        TemporaryFile& operator=(const TemporaryFile&) = delete;

        ~TemporaryFile() {
            if (handle != INVALID_HANDLE_VALUE) {
                CloseHandle(handle);
            }

            if (!path.empty()) {
                DeleteFileW(path.c_str());
            }
        }
    };
}


namespace FileIO {
    bool readFileOneShot(const std::filesystem::path& inputPath, std::string& outPayload, std::size_t maxBytes, std::string& error) {
        error.clear();

        // Read raw bytes; ate initially positions the stream at the end.
        std::ifstream file(inputPath, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            error = "Cannot open file";
            return false;
        }

        // At the end of the file, the current position gives its size in bytes.
        const std::streamoff size = file.tellg();
        if (size < 0) {
            error = "Cannot determine file size";
            return false;
        }

        // Check the size before allocating memory for the contents.
        const std::uintmax_t byteCount = static_cast<std::uintmax_t>(size);
        if (byteCount > maxBytes || byteCount > static_cast<std::uintmax_t>((std::numeric_limits<std::streamsize>::max)())) {
            error = "File exceeds the allowed size";
            return false;
        }

        // Prepare a local buffer so failed reads leave outPayload unchanged.
        std::string payload(static_cast<std::size_t>(size), '\0');

        // Return to the beginning before reading the measured number of bytes.
        file.seekg(0, std::ios::beg);
        if (!file) {
            error = "Cannot seek to the beginning of the file";
            return false;
        }

        if (!payload.empty() && !file.read(payload.data(), static_cast<std::streamsize>(payload.size()))) {
            error = "Cannot read the complete file";
            return false;
        }

        // Publish the completed read without copying the entire buffer.
        outPayload.swap(payload);
        return true;
    }


    bool writeFileAtomically(const std::filesystem::path& path, std::string_view contents, std::string& error) {
        error.clear();

        if (path.empty()) {
            error = "Destination path is empty";
            return false;
        }

        // Get the full destination path.
        std::error_code ec;
        const std::filesystem::path destination = std::filesystem::absolute(path, ec);
        if (ec) {
            error = "Invalid destination path: " + ec.message();
            return false;
        }

        // Create any missing parent folders.
        std::filesystem::create_directories(destination.parent_path(), ec);
        if (ec) {
            error = "Cannot create destination directory: " + ec.message();
            return false;
        }

        // Use a counter to help generate unique temporary names.
        static std::atomic<std::uint64_t> nextId{0};
        TemporaryFile temporary;

        // Try up to 32 temporary filenames.
        for (int attempt = 0; attempt < 32; ++attempt) {
            std::filesystem::path candidate = destination;
            candidate += L".tmp." + std::to_wstring(GetCurrentProcessId()) + L"." + std::to_wstring(nextId.fetch_add(1));

            temporary.path = std::move(candidate);

            // Create a new temporary file without overwriting an existing one.
            temporary.handle = CreateFileW(
                temporary.path.c_str(),
                GENERIC_WRITE,
                0,
                nullptr,
                CREATE_NEW,
                FILE_ATTRIBUTE_NORMAL,
                nullptr);

            if (temporary.handle != INVALID_HANDLE_VALUE) {
                break;
            }

            // Save the Windows error immediately.
            const DWORD code = GetLastError();

            // Do not delete a file we did not create.
            temporary.path.clear();

            if (code != ERROR_FILE_EXISTS && code != ERROR_ALREADY_EXISTS) {
                error = "Cannot create temporary file: " + windowsError(code);
                return false;
            }
        }

        if (temporary.handle == INVALID_HANDLE_VALUE) {
            error = "Cannot find a unique temporary filename";
            return false;
        }

        // Keep writing until all bytes are saved.
        std::size_t offset = 0;

        while (offset < contents.size()) {
            const DWORD requested = (std::min)(contents.size() - offset,static_cast<std::size_t>((std::numeric_limits<DWORD>::max)()));

            DWORD written = 0;

            if (!WriteFile(temporary.handle, contents.data() + offset, requested, &written, nullptr)) {
                const DWORD code = GetLastError();
                error = "Cannot write temporary file: " + windowsError(code);
                return false;
            }

            // Stop if the write makes no progress.
            if (written == 0) {
                error = "Writing temporary file made no progress";
                return false;
            }

            offset += written;
        }

        // Flush buffered data before replacing the destination.
        if (!FlushFileBuffers(temporary.handle)) {
            const DWORD code = GetLastError();
            error = "Cannot flush temporary file: " + windowsError(code);
            return false;
        }

        // Close the file before moving it.
        if (!CloseHandle(temporary.handle)) {
            const DWORD code = GetLastError();
            error = "Cannot close temporary file: " + windowsError(code);
            return false;
        }

        // Prevent cleanup from closing the handle twice.
        temporary.handle = INVALID_HANDLE_VALUE;

        // Replace the destination with the completed temporary file.
        if (!MoveFileExW(temporary.path.c_str(), destination.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
            const DWORD code = GetLastError();
            error = "Cannot replace destination file: " + windowsError(code);
            return false;
        }

        // The temporary file was moved, so no deletion is needed.
        temporary.path.clear();
        return true;
    }
}
