#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

float maxf(float x, float other)
{
	return (x > other) ? x : other;
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
	float clockwise = 1.0f;

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-d") == 0 && (i + 1 < argc))
		{
			duration = maxf(std::stof(argv[i + 1]), 0.0f);
		}
		else if (strcmp(argv[i], "-c") == 0 && (i + 1 < argc))
		{
			if (strcmp(argv[i + 1], "true") == 0)
			{
				clockwise = 1.0f;
			}
			else if (strcmp(argv[i + 1], "false") == 0)
			{
				clockwise = -1.0f;
			}
		}
	}

	std::string loadname = "template.gltf";
    std::string savename = "generated.gltf";
    std::string binary = "generated.bin";

    std::string templateContent;
	if (!loadFile(templateContent, loadname))
	{
		printf("Error: Could not load template flTF file '%s'\n", loadname.c_str());

		return -1;
	}

    json glTF = json::parse(templateContent);
    glTF["accessors"][0]["max"][0] = duration;

    //

    std::vector<float> floatData;

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

    floatData.push_back(+0.000f);
    floatData.push_back(+0.707f);
    floatData.push_back(+0.000f);
    floatData.push_back(-clockwise * +0.707f);

    floatData.push_back(+0.000f);
    floatData.push_back(+1.000f);
    floatData.push_back(+0.000f);
    floatData.push_back(+0.000f);

    floatData.push_back(+0.000f);
    floatData.push_back(+0.707f);
    floatData.push_back(+0.000f);
    floatData.push_back(clockwise * +0.707f);

    floatData.push_back(+0.000f);
    floatData.push_back(+0.000f);
    floatData.push_back(+0.000f);
    floatData.push_back(-1.000f);

    std::string data;
    data.resize(floatData.size() * sizeof(float));
    memcpy(data.data(), floatData.data(), floatData.size() * sizeof(float));

	if (!saveFile(data, binary))
	{
		printf("Error: Could not save generated binary file '%s' \n", binary.c_str());

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
