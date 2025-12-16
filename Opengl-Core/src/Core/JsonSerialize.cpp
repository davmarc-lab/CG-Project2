#include "../../include/Core/JsonSerialize.hpp"

#include <fstream>

void JsonSerializer::serializeToFile(Json::Value &elem, const std::string &filePath) {
	if (elem == Json::nullValue)
		return;

	Json::StyledWriter writer{};

	std::ofstream out(filePath);
	out << writer.write(elem);
	out.close();
}
