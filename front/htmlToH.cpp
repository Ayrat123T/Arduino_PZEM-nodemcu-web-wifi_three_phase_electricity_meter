#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <windows.h>
#include <stdlib.h>

std::string WebToStr(std::ifstream& index_html_in) {
    std::string result;
    std::string line;
    if (index_html_in.is_open()) {
        while (std::getline(index_html_in, line)) {
            char last_line_element = line[line.size() - 1];
            if (last_line_element == ';' || last_line_element == '>' || last_line_element == '{' || last_line_element == '}') {
                result +="\n";
            }
            if (line.find("<link rel=\"stylesheet\"") != -1) {
                line.clear();
                line = "<style type=\"text/css\">";
                std::ifstream index_html_in("style.css");
                line += WebToStr(index_html_in);
                line += "</style>";
            }
            if (line.find("<script src=") != -1) {
                line.clear();
                line = "<script>";
                std::ifstream index_html_in("meterCheckScript.js");
                line += WebToStr(index_html_in);
                line += "</script>";
            }
            result += line;
        }
    }
    return result;
}

int main(int argc, char*argv[]) {
    std::ifstream index_html_in("meterCheck.html"); // окрываем файл для чтения
    std::string html = "const char webpage[] PROGMEM = R\"=====(";
    html += WebToStr(index_html_in);
    html += ")=====\";";
    TCHAR buffer[MAX_PATH];
    GetCurrentDirectory(sizeof(buffer),buffer); //Получить текущую деректорию
    const std::filesystem::path comtrade_files_path = std::filesystem::path(buffer).parent_path();
    std::ofstream index_h_out(comtrade_files_path / "srs/PZEM_nodemcu_three_phase/index.h"); // окрываем файл для записи
       if (index_h_out.is_open())
    {
        index_h_out << html;
    }
    index_h_out.close();
    return 0;
}