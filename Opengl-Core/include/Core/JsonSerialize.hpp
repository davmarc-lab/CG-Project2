#include "Serialize.hpp"

#include <json/json.h>

class JsonSerializable : public Serializable<Json::Value> {
	virtual ~JsonSerializable() = default;

	virtual Json::Value serialize() { return Json::nullValue; }

	virtual void deserialize(Json::Value &elem) {}
};

class JsonSerializer : public Serializer<Json::Value> {
public:
	virtual ~JsonSerializer() = default;

	virtual void serializeToFile(Json::Value &elem, const std::string &path) override;
};
