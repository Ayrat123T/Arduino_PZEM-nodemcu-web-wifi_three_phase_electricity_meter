#include "htmlToH.h"

std::string WebToStr(std::ifstream& index_html_in) {
    const std::filesystem::path CurrentPath = std::filesystem::current_path().parent_path().parent_path();
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
                std::ifstream index_html_in(CurrentPath / "front/style.css");
                line += WebToStr(index_html_in);
                line += "\n</style>";
            }
            if (line.find("<script src=") != -1) {
                line.clear();
                line = "<script>";
                std::ifstream index_html_in(CurrentPath / "front/meterCheckScript.js");
                line += WebToStr(index_html_in);
                line += "\n</script>";
            }
            result += line;
        }
    }
    return result;
}

void MakeStrFromWeb() {
    std::string html = "const char webpage[] PROGMEM = R\"=====(";
    const std::filesystem::path CurrentPath = std::filesystem::current_path().parent_path().parent_path();
    std::ifstream index_html_in(CurrentPath / "front/meterCheck.html"); // окрываем файл для чтения
    std::ofstream index_h_out("index.h"); // окрываем файл для записи
    html += WebToStr(index_html_in);
    html += ")=====\";";
    if (index_h_out.is_open()) {
        index_h_out << html;
    }
    index_h_out.close();
}