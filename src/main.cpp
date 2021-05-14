#include <cmath>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

constexpr float PI = 3.14159265358979323846f;

struct Quaternion
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 1.0f;
};

float maxf(float x, float other)
{
	return (x > other) ? x : other;
}

float minf(float x, float other)
{
	return (x < other) ? x : other;
}

float clampf(float x, float minVal, float maxVal)
{
	float t = (x > minVal) ? x : minVal;

	return (t < maxVal) ? t : maxVal;
}

float radiansf(float degrees)
{
	return (PI * degrees) / 180.0f;
}

float degreesf(float radians)
{
	return 180.0f * radians / PI;
}

Quaternion eulerToQuaternionf(float xRadians, float yRadians, float zRadians)
{
    float cx = cosf(xRadians * 0.5);
    float sx = sinf(xRadians * 0.5);
    float cy = cosf(yRadians * 0.5);
    float sy = sinf(yRadians * 0.5);
    float cz = cosf(zRadians * 0.5);
    float sz = sinf(zRadians * 0.5);

    Quaternion q;
    q.x = sx * cy * cz - cx * sy * sz;
    q.y = cx * sy * cz + sx * cy * sz;
    q.z = cx * cy * sz - sx * sy * cz;
    q.w = cx * cy * cz + sx * sy * sz;

    return q;
}

bool loadFile(std::string& output, const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);
	if (!file.is_open())
	{
		return false;
	}

	size_t fileSize = static_cast<size_t>(file.tellg());
	file.seekg(0);

	output.resize(fileSize);

	file.read(output.data(), fileSize);
	file.close();

	return true;
}

bool saveFile(const std::string& output, const std::string& filename)
{
	std::ofstream file(filename, std::ios::binary);
	if (!file.is_open())
	{
		return false;
	}

	file.write(output.data(), output.size());
	file.close();

	return true;
}

int main(int argc, char *argv[])
{
	std::string operation = "rotation";

	float duration = 2.0f;

	float x = 0.0f;
	float y = 1.0f;
	float z = 0.0f;

	bool bounce = false;

	float degree = 360.0f;

	//

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-o") == 0 && (i + 1 < argc))
		{
			if (strcmp(argv[i + 1], "rotation") == 0)
			{
				operation = "rotation";
			}
			else if (strcmp(argv[i + 1], "translation") == 0)
			{
				operation = "translation";
			}
			else
			{
				printf("Error: Unknown operation '%s'\n", argv[i + 1]);

				return -1;
			}
		}
		else if (strcmp(argv[i], "-t") == 0 && (i + 1 < argc))
		{
			duration = maxf(std::stof(argv[i + 1]), 0.0f);
		}
		else if (strcmp(argv[i], "-x") == 0 && (i + 1 < argc))
		{
			x = std::stof(argv[i + 1]);
		}
		else if (strcmp(argv[i], "-y") == 0 && (i + 1 < argc))
		{
			y = std::stof(argv[i + 1]);
		}
		else if (strcmp(argv[i], "-z") == 0 && (i + 1 < argc))
		{
			z = std::stof(argv[i + 1]);
		}
		else if (strcmp(argv[i], "-b") == 0 && (i + 1 < argc))
		{
			if (strcmp(argv[i + 1], "true") == 0)
			{
				bounce = true;
			}
			else if (strcmp(argv[i + 1], "false") == 0)
			{
				bounce = false;
			}
			else
			{
				printf("Warning: Unknown bounce value '%s'\n", argv[i + 1]);
			}
		}
		else if (strcmp(argv[i], "-d") == 0 && (i + 1 < argc))
		{
			degree = clampf(std::stof(argv[i + 1]), -360.0f, 360.0f);
		}
	}

	//

	if (operation == "rotation")
	{
		float length = sqrtf(x*x + y*y + z*z);
		x /= length;
		y /= length;
		z /= length;
	}

	//

	std::string generatorname = operation;

	generatorname += "_t" + std::to_string(duration);

	generatorname += "_x" + std::to_string(x);
	generatorname += "_y" + std::to_string(y);
	generatorname += "_z" + std::to_string(z);

	generatorname += "_b" + std::string(bounce ? "true" : "false");

	generatorname += "_d" + std::to_string(degree);

	std::string loadname = "template.gltf";
    std::string savename = generatorname + ".gltf";
    std::string binaryname = generatorname + ".bin";

    std::string templateContent;
	if (!loadFile(templateContent, loadname))
	{
		printf("Error: Could not load template glTF file '%s'\n", loadname.c_str());

		return -1;
	}

	//

    json glTF = json::parse(templateContent);
    glTF["accessors"][0]["max"][0] = duration;
	glTF["animations"][0]["channels"][0]["target"]["path"] = operation;
    glTF["buffers"][0]["uri"] = binaryname;
    glTF["nodes"][0]["name"] = generatorname;

    //

    std::vector<float> floatData;
	size_t loop;

    if (operation == "rotation")
    {
    	loop = 5;
    	if (bounce)
    	{
    		loop += 4;
    	}
    	glTF["accessors"][0]["count"] = loop;
    	glTF["accessors"][1]["count"] = loop;
    	glTF["accessors"][1]["type"] = "VEC4";

    	float divisor = static_cast<float>(loop - 1);

		// Timepoint
		for (size_t i = 0; i < loop; i++)
		{
			floatData.push_back(duration * static_cast<float>(i) / divisor);
		}

		// Quaternion
		Quaternion q;

		floatData.push_back(+0.0f);
		floatData.push_back(+0.0f);
		floatData.push_back(+0.0f);
		floatData.push_back(+1.0f);

		q = eulerToQuaternionf(radiansf(degree * 0.25f) * x, radiansf(degree * 0.25f) * y, radiansf(degree * 0.25f) * z);
		floatData.push_back(q.x);
		floatData.push_back(q.y);
		floatData.push_back(q.z);
		floatData.push_back(q.w);

		q = eulerToQuaternionf(radiansf(degree * 0.5f) * x, radiansf(degree * 0.5f) * y, radiansf(degree * 0.5f) * z);
		floatData.push_back(q.x);
		floatData.push_back(q.y);
		floatData.push_back(q.z);
		floatData.push_back(q.w);

		q = eulerToQuaternionf(radiansf(degree * 0.75f) * x, radiansf(degree * 0.75f) * y, radiansf(degree * 0.75f) * z);
		floatData.push_back(q.x);
		floatData.push_back(q.y);
		floatData.push_back(q.z);
		floatData.push_back(q.w);

		q = eulerToQuaternionf(radiansf(degree * 1.0f) * x, radiansf(degree * 1.0f) * y, radiansf(degree * 1.0f) * z);
		floatData.push_back(q.x);
		floatData.push_back(q.y);
		floatData.push_back(q.z);
		floatData.push_back(q.w);

		if (bounce)
		{
			q = eulerToQuaternionf(radiansf(degree * 0.75f) * x, radiansf(degree * 0.75f) * y, radiansf(degree * 0.75f) * z);
			floatData.push_back(q.x);
			floatData.push_back(q.y);
			floatData.push_back(q.z);
			floatData.push_back(q.w);

			q = eulerToQuaternionf(radiansf(degree * 0.5f) * x, radiansf(degree * 0.5f) * y, radiansf(degree * 0.5f) * z);
			floatData.push_back(q.x);
			floatData.push_back(q.y);
			floatData.push_back(q.z);
			floatData.push_back(q.w);

			q = eulerToQuaternionf(radiansf(degree * 0.25f) * x, radiansf(degree * 0.25f) * y, radiansf(degree * 0.25f) * z);
			floatData.push_back(q.x);
			floatData.push_back(q.y);
			floatData.push_back(q.z);
			floatData.push_back(q.w);

			floatData.push_back(+0.0f);
			floatData.push_back(+0.0f);
			floatData.push_back(+0.0f);
			floatData.push_back(+1.0f);
		}
    }
    else if (operation == "translation")
    {
    	loop = 3;
    	if (bounce)
    	{
    		loop += 2;
    	}
    	glTF["accessors"][0]["count"] = loop;
    	glTF["accessors"][1]["count"] = loop;
    	glTF["accessors"][1]["type"] = "VEC3";

    	float divisor = static_cast<float>(loop - 1);

		// Timepoint
		for (size_t i = 0; i < loop; i++)
		{
			floatData.push_back(duration * static_cast<float>(i) / divisor);
		}

		// Vec3
		floatData.push_back(0.0f);
		floatData.push_back(0.0f);
		floatData.push_back(0.0f);

		floatData.push_back(0.5f * x);
		floatData.push_back(0.5f * y);
		floatData.push_back(0.5f * z);

		floatData.push_back(x);
		floatData.push_back(y);
		floatData.push_back(z);

		if (bounce)
		{
			floatData.push_back(0.5f * x);
			floatData.push_back(0.5f * y);
			floatData.push_back(0.5f * z);

			floatData.push_back(0.0f);
			floatData.push_back(0.0f);
			floatData.push_back(0.0f);
		}
    }

    std::string data;
    data.resize(floatData.size() * sizeof(float));
    memcpy(data.data(), floatData.data(), floatData.size() * sizeof(float));

    glTF["bufferViews"][0]["byteLength"] = loop * sizeof(float);
    glTF["bufferViews"][0]["byteOffset"] = 0;
    glTF["bufferViews"][1]["byteLength"] = data.size() - loop * sizeof(float);
    glTF["bufferViews"][1]["byteOffset"] = loop * sizeof(float);

    glTF["buffers"][0]["byteLength"] = data.size();

	if (!saveFile(data, binaryname))
	{
		printf("Error: Could not save generated binary file '%s' \n", binaryname.c_str());

		return -1;
	}

    //

	if (!saveFile(glTF.dump(3), savename))
	{
		printf("Error: Could not save generated glTF file '%s' \n", savename.c_str());

		return -1;
	}

	return 0;
}
