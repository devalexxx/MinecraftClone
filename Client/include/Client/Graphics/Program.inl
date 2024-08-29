
#include <Client/Graphics/Common.h>
#include <fmt/base.h>

namespace Mcc
{
	template<glm::length_t R, glm::length_t C, glm::qualifier Q>
	void Program::SetUniformMatrix(GLint location, const glm::mat<R, C, float, Q>& matrix)
	{
		if (sUsedProgram != mId)
			Use();

		if (R == C)
		{
			switch (R)
			{
				case 2:
					glCheck(glUniformMatrix2fv(location, 1, GL_FALSE, &matrix[0][0]));
					break;
				case 3:
					glCheck(glUniformMatrix3fv(location, 1, GL_FALSE, &matrix[0][0]));
					break;
				case 4:
					glCheck(glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]));
					break;
				default:
					fmt::print("glm::mat<{}, {}> is not implemented for Program::SetUniformMatrix\n", R, C);
			}
		}
		else
		{
			fmt::print("glm::mat<{}, {}> is not implemented for Program::SetUniformMatrix\n", R, C);
		}
	}
}