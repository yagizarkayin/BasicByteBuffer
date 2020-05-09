#include "BasicByteBuffer.h"

#include <iostream>

struct Vec3
{
	float x;
	float y;
	float z;

	void print()
	{
		std::cout << "X: " << x << "Y: " << y << "Z: " << z << std::endl;
	}
};

enum Color
{
	BANANA = 1,
	APPLE = 2,
	ORANGE = 3
};



class VeryLargeClass
{
public:
	VeryLargeClass() :
		meshPoints(256)
	{ 
		std::cout << "This is a very large class" << std::endl;
	}

	void increaseAll()
	{
		for (auto& mesh : meshPoints)
		{
			mesh.x = 3.2f;
			mesh.y = -2.6f;
			mesh.z = 1.1f;
		}
	}

	void printAll()
	{
		for (auto& mesh : meshPoints)
		{
			mesh.print();
		}
	}

private:
	std::vector<Vec3> meshPoints;
};

int main()
{
	Vec3 out_singlePoint{ 0.2f, -0.3f, 1.5f };
	const double out_doubleValue = 3579.246;

	Color out_myColor = Color::BANANA;

	VeryLargeClass out_myVeryLargeClassInstance;
	out_myVeryLargeClassInstance.increaseAll();
	out_myVeryLargeClassInstance.printAll();

	BasicByteWriter writer;

	writer.write(out_singlePoint);
	writer.write(out_doubleValue);
	writer.write(out_myColor);
	writer.write(out_myVeryLargeClassInstance);

	const auto result = std::move(writer.getDataAndResetBuffer());

	BasicByteReader reader(result.data(), result.size());

	Vec3 in_singlePoint;
	double in_doubleValue;
	Color in_myColor;
	VeryLargeClass* in_myVeryLargeClassInstance;

	reader.read(in_singlePoint);
	reader.read(in_doubleValue);
	reader.read(in_myColor);
	reader.read(in_myVeryLargeClassInstance);

	in_myVeryLargeClassInstance->printAll();

	std::cin.get();
	return 0;
}