#include "../../include/Core/JsonSerialize.hpp"

#include <fstream>

Json::Value JsonSerializer::deserializeFromFile(const std::string &path) {
	std::ifstream f(path);

    Json::CharReaderBuilder builder;
    std::string errs;
	Json::Value root;

	Json::parseFromStream(builder, f, &root, &errs);

	f.close();
	if (!errs.empty())
		return Json::nullValue;
    
	return root;
}

void JsonSerializer::serializeToFile(Json::Value &elem, const std::string &filePath) {
	if (elem == Json::nullValue)
		return;

	Json::StreamWriterBuilder builder{};

	std::ofstream out(filePath);
	out << Json::writeString(builder, elem);
	out.close();
}
