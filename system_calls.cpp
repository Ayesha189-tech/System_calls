#include <iostream>
    #include <fstream>
    #include <cstdlib>
    #include <string>
    #include <windows.h> // Windows-specific header
    
    using namespace std;
    
    int main() {
        cout << "Content-Type: text/html\n\n";
        cout << "<!DOCTYPE html>";
        cout << "<html lang='en'><head>";
        cout << "<meta charset='UTF-8'>";
        cout << "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
        cout << "<title>System Calls in C++ CGI</title>";
        cout << "<link href='https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css' rel='stylesheet'>";
        cout << "<style>";
        cout << "body { background-color: #f8f9fa; font-family: Arial, sans-serif; }";
        cout << ".container { max-width: 800px; margin-top: 50px; padding: 20px; background: #fff; border-radius: 10px; box-shadow: 0px 0px 10px rgba(0,0,0,0.1); }";
        cout << ".output-box { border-left: 5px solid #007bff; padding: 15px; background: #eef2ff; margin-bottom: 20px; }";
        cout << "h1, h2 { color: #0056b3; }";
        cout << "a { text-decoration: none; font-weight: bold; }";
        cout << "</style>";
        cout << "</head><body>";
    
        cout << "<div class='container'>";
        cout << "<h1 class='text-center'>System Calls</h1>";
    
        string query = getenv("QUERY_STRING");
        string filename = "default.txt";
    
        size_t pos = query.find("filename=");
        if (pos != string::npos) {
            filename = query.substr(pos + 9);
            size_t end = filename.find("&");
            if (end != string::npos) filename = filename.substr(0, end);
        }
    
        cout << "<div class='output-box'>";
        string filepath = "C:\\xampp\\htdocs\\" + filename; // Windows XAMPP ke liye
        ofstream file(filepath, ios::app);
        
        // *File System Calls*
        cout << "<h2>File System Calls</h2>";
    if (query.find("action=open") != string::npos) {
        ofstream file(filename, ios::app);
        if (file) {
            file << "This is " << filename << " file.\n";
            file.close();
            cout << "<p><strong>File Created:</strong> '" << filename << "' created.</p>";
            cout << "<p><a href='" << filename << "' target='_blank'>Open " << filename << "</a></p>";
    
            // Open file using default program based on extension
            string command = "start \"\" \"" + filename + "\"";
            system(command.c_str());  // Open file with default application
        } else {
            cout << "<p><strong>Error:</strong> Unable to create file.</p>";
        }
    }
    
        
        else if (query.find("action=write") != string::npos) {  
            ofstream file(filename, ios::app);
            if (file) {
                file << "New content added to the file.\n";
                file.close();
                cout << "<p><strong>Write Success:</strong> Content added to '" << filename << "'.</p>";
            } else {
                cout << "<p><strong>Error:</strong> Unable to write to file.</p>";
            }
        } 
        else if (query.find("action=read") != string::npos) {
            ifstream file(filename);
            if (file) {
                string content, line;
                while (getline(file, line)) {
                    content += line + "<br>";
                }
                file.close();
                cout << "<p><strong>File Contents of:</strong> '" << filename << "'</p>";
                cout << "<p>" << content << "</p>";
            } else {
                cout << "<p><strong>Error:</strong> File not found.</p>";
            }
        } 
        else if (query.find("action=close") != string::npos) {
            if (remove(filename.c_str()) == 0) {
                cout << "<p><strong>File Deleted:</strong> '" << filename << "' removed successfully.</p>";
            } else {
                cout << "<p><strong>Error:</strong> Unable to delete file.</p>";
            }
        }
    
        // *Process Creation (Windows Alternative to fork)*
        cout << "<h2>Process Control Calls</h2>";
        if (query.find("action=fork") != string::npos) {
            STARTUPINFO si = { sizeof(si) };
            PROCESS_INFORMATION pi;
            if (CreateProcess(NULL, (LPSTR)"cmd.exe", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
                cout << "<p>Child process created successfully.</p>";
                WaitForSingleObject(pi.hProcess, INFINITE);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            } else {
                cout << "<p><strong>Error:</strong> Process creation failed.</p>";
            }
        }
    
        // *Memory Allocation (Windows Alternative to mmap)*
        cout << "<h2>Memory Management Calls</h2>";
        if (query.find("action=mmap") != string::npos) {
            void* addr = VirtualAlloc(NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            if (addr != NULL) {
                cout << "<p>Memory allocated successfully.</p>";
                VirtualFree(addr, 0, MEM_RELEASE);
            } else {
                cout << "<p><strong>Error:</strong> Memory allocation failed.</p>";
            }
        }
    
        cout << "</div>";
        cout << "<div class='text-center'>";
        cout << "<a class='btn btn-primary' href='/index.html'>Go Back</a>";
        cout << "</div>";
        cout << "</div>";
        cout << "</body></html>";
    
        return 0;
    }