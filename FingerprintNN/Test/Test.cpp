#include "pch.h"
#include "CppUnitTest.h"
#include <string>
#include "src/Matrix.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test
{
	TEST_CLASS(Test)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			vecf input(2);
			input(0) = 1; input(1) = 0;


			matf w1(2, 2);
			
			w1(0, 0) = 5.30; w1(0, 1) = 5.31;
			w1(1, 0) = 3.87; w1(1, 1) = 3.87;

			matf w2(2, 1);

			w2(0, 0) = 14.751; w2(1, 0) = -16.373;


			vecf b1(2);
			b1(0) = -2.37; b1(1) = -6.16;

			
			vecf b2(1);
			b2(0) = -6.8;


			vecf o1 = w1 * input +b1;

			for (int i = 0; i < o1.size(); ++i)
				o1(i) = 1.0f / (1 + std::powf(2.71825, -o1(i)));


			vecf o2 = w2 * o1 + b2;

			for (int i = 0; i < o2.size(); ++i)
				o2(i) = 1.0f / (1 + std::powf(2.71825, -o2(i)));


			Assert::IsTrue(o2(0) > 0.9);	
			Logger::WriteMessage(std::to_string(o2(0)).c_str());			
		}
	};
}
