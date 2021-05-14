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

float radiansf(float degrees)
{
	return (PI * degrees) / 180.0f;
}

float degreesf(float radians)
{
	return 180.0f * radians / PI;
}

Quaternion eulerToQuaternionf(double xRadians, double yRadians, double zRadians)
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
	float duration = 2.0f;
	std::string operation = "rotation";

	float direction = 1.0f;
	std::string axis = "y";

	float end = 2.0f;
	float offset = 0.0f;

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
				printf("Warning: Unknown operation '%s'\n", argv[i + 1]);
			}
		}
		else if (strcmp(argv[i], "-t") == 0 && (i + 1 < argc))
		{
			duration = maxf(std::stof(argv[i + 1]), 0.0f);
		}
		else if (strcmp(argv[i], "-a") == 0 && (i + 1 < argc))
		{
			if (strcmp(argv[i + 1], "x") == 0)
			{
				axis = "x";
			}
			else if (strcmp(argv[i + 1], "y") == 0)
			{
				axis = "y";
			}
			else if (strcmp(argv[i + 1], "z") == 0)
			{
				axis = "z";
			}
			else
			{
				printf("Warning: Unknown axis '%s'\n", argv[i + 1]);
			}
		}
		else if (strcmp(argv[i], "-d") == 0 && (i + 1 < argc))
		{
			if (strcmp(argv[i + 1], "cw") == 0)
			{
				direction = 1.0f;
			}
			else if (strcmp(argv[i + 1], "ccw") == 0)
			{
				direction = -1.0f;
			}
			else
			{
				printf("Warning: Unknown direction '%s'\n", argv[i + 1]);
			}
		}
		else if (strcmp(argv[i], "-e") == 0 && (i + 1 < argc))
		{
			end = std::stof(argv[i + 1]);
		}
		else if (strcmp(argv[i], "-b") == 0 && (i + 1 < argc))
		{
			if (strcmp(argv[i + 1], "true") == 0)
			{
				offset = end * 0.5f;
			}
			else if (strcmp(argv[i + 1], "false") == 0)
			{
				offset = 0.0;
			}
			else
			{
				printf("Warning: Unknown bounce value '%s'\n", argv[i + 1]);
			}
		}
	}

	std::string generatorname = operation;

	generatorname += "_" + axis;

	if (operation == "rotation")
	{
		if (direction == 1.0f)
		{
			generatorname += "_cw";
		}
		else
		{
			generatorname += "_ccw";
		}
	}
	else if (operation == "translation")
	{
		if (direction == 1.0f)
		{
			generatorname += "_pos";
		}
		else
		{
			generatorname += "_neg";
		}

		if (offset == 0.0f)
		{
			generatorname += "_oneway";
		}
		else
		{
			generatorname += "_bounce";
		}
	}

	generatorname += "_" + std::to_string(duration);

	std::string loadname = "template_" + operation + ".gltf";
    std::string savename = generatorname + ".gltf";
    std::string binaryname = generatorname + ".bin";

    std::string templateContent;
	if (!loadFile(templateContent, loadname))
	{
		printf("Error: Could not load template glTF file '%s'\n", loadname.c_str());

		return -1;
	}

    json glTF = json::parse(templateContent);
    glTF["accessors"][0]["max"][0] = duration;
    glTF["buffers"][0]["uri"] = binaryname;
    glTF["nodes"][0]["name"] = generatorname;

    //

    std::vector<float> floatData;

    if (operation == "rotation")
    {
		// Timepoint
		for (size_t i = 0; i < 5; i++)
		{
			floatData.push_back(duration * static_cast<float>(i) / 4.0f);
		}

		// Quaternion
		floatData.push_back(+0.000f);
		floatData.push_back(+0.000f);
		floatData.push_back(+0.000f);
		floatData.push_back(+1.000f);

		if (axis == "x")
		{
			floatData.push_back(-direction * +0.707f);
			floatData.push_back(+0.000f);
			floatData.push_back(+0.000f);
			floatData.push_back(+0.707f);

			floatData.push_back(+1.000f);
			floatData.push_back(+0.000f);
			floatData.push_back(+0.000f);
			floatData.push_back(+0.000f);

			floatData.push_back(direction * +0.707f);
			floatData.push_back(+0.000f);
			floatData.push_back(+0.000f);
			floatData.push_back(+0.707f);
		}
		else if (axis == "y")
		{
			floatData.push_back(+0.000f);
			floatData.push_back(+0.707f);
			floatData.push_back(+0.000f);
			floatData.push_back(-direction * +0.707f);

			floatData.push_back(+0.000f);
			floatData.push_back(+1.000f);
			floatData.push_back(+0.000f);
			floatData.push_back(+0.000f);

			floatData.push_back(+0.000f);
			floatData.push_back(+0.707f);
			floatData.push_back(+0.000f);
			floatData.push_back(direction * +0.707f);
		}
		else if (axis == "z")
		{
			floatData.push_back(+0.000f);
			floatData.push_back(+0.000f);
			floatData.push_back(+0.707f);
			floatData.push_back(-direction * +0.707f);

			floatData.push_back(+0.000f);
			floatData.push_back(+0.000f);
			floatData.push_back(+1.000f);
			floatData.push_back(+0.000f);

			floatData.push_back(+0.000f);
			floatData.push_back(+0.000f);
			floatData.push_back(+0.707f);
			floatData.push_back(direction * +0.707f);
		}

		floatData.push_back(+0.000f);
		floatData.push_back(+0.000f);
		floatData.push_back(+0.000f);
		floatData.push_back(+1.000f);
    }
    else if (operation == "translation")
    {
		// Timepoint
		for (size_t i = 0; i < 3; i++)
		{
			floatData.push_back(duration * static_cast<float>(i) / 2.0f);
		}

		// Vec3
		floatData.push_back(+0.000f);
		floatData.push_back(+0.000f);
		floatData.push_back(+0.000f);

		if (axis == "x")
		{
			floatData.push_back(end * 0.5f + offset);
			floatData.push_back(+0.000f);
			floatData.push_back(+0.000f);

			floatData.push_back(end - 2.0f * offset);
			floatData.push_back(+0.000f);
			floatData.push_back(+0.000f);
		}
		else if (axis == "y")
		{
			floatData.push_back(+0.000f);
			floatData.push_back(end * 0.5f + offset);
			floatData.push_back(+0.000f);

			floatData.push_back(+0.000f);
			floatData.push_back(end - 2.0f * offset);
			floatData.push_back(+0.000f);
		}
		else if (axis == "z")
		{
			floatData.push_back(+0.000f);
			floatData.push_back(+0.000f);
			floatData.push_back(end * 0.5f + offset);

			floatData.push_back(+0.000f);
			floatData.push_back(+0.000f);
			floatData.push_back(end - 2.0f * offset);
		}
    }

    std::string data;
    data.resize(floatData.size() * sizeof(float));
    memcpy(data.data(), floatData.data(), floatData.size() * sizeof(float));

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
