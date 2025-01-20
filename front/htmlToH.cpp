#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
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
                line += "\n</style>";
            }
            if (line.find("<script src=") != -1) {
                line.clear();
                line = "<script>";
                std::ifstream index_html_in("script.js");
                line += WebToStr(index_html_in);
                line += "\n</script>";
            }
            result += line;
        }
    }
    return result;
}

std::string MakeStrFromWeb() {
    std::ifstream index_html_in("index.html"); // открываем файл для чтения
    std::string html = "const char webpage[] PROGMEM = R\"=====(";
    html += WebToStr(index_html_in);
    html += ")=====\";";
    const std::filesystem::path CurrentPath = std::filesystem::current_path().parent_path();
    std::ofstream index_h_out(CurrentPath/ "srs/PZEM_nodemcu_three_phase/index.h"); // открываем файл для записи
    if (index_h_out.is_open()) {
        index_h_out << html;
    }
    index_h_out.close();
    return html;
}

int main() {
    std::string html = MakeStrFromWeb();
    return 0;
}