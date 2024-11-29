#include <string>
#include <iostream>
#include <fstream>

std::string WebToStr(std::ifstream& index_html_in) {
    std::string result;
    std::string line;
    if (index_html_in.is_open()) {
        while (std::getline(index_html_in, line)) {
            if (line.find("<link rel=\"stylesheet\"") != -1) {
                line.clear();
                line = "<style type=\"text/css\">";
                std::ifstream index_html_in("front/style.css");
                line += WebToStr(index_html_in);
                line += "</style>";
            }
            if (line.find("<script src=") != -1) {
                line.clear();
                line = "<script>";
                std::ifstream index_html_in("front/meterCheckScript.js");
                line += WebToStr(index_html_in);
                line += "</script>";
            }
            result += line;
        }
    }
    return result;
}

int main() {
    std::ifstream index_html_in("front/meterCheck.html"); // окрываем файл для чтения
    std::string html = "const char webpage[] PROGMEM = R\"=====(";
    html += WebToStr(index_html_in);
    html += ")=====\";";
    std::ofstream index_h_out("srs/PZEM_nodemcu_three_phase/index.h"); // окрываем файл для записи
       if (index_h_out.is_open())
    {
        index_h_out << html;
    }
    index_h_out.close();
    return 0;
}