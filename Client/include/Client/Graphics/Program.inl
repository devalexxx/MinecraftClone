// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#include "Client/Graphics/Common.h"

#include "Common/Utils/Logging.h"

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
                    MCC_LOG_WARN("glm::mat<{}, {}> is not implemented for Program::SetUniformMatrix", R, C);
            }
        }
        else
        {
            MCC_LOG_WARN("glm::mat<{}, {}> is not implemented for Program::SetUniformMatrix", R, C);
        }
    }

    template<glm::length_t L, glm::qualifier Q>
    void Program::SetUniformVector(GLint location, const glm::vec<L, float, Q>& vec)
    {
        if (sUsedProgram != mId)
            Use();

        switch (L)
        {
            case 2:
                glCheck(glUniform2fv(location, 1, &vec[0]));
                break;
            case 3:
                glCheck(glUniform3fv(location, 1, &vec[0]));
                break;
            case 4:
                glCheck(glUniform4fv(location, 1, &vec[0]));
                break;
            default:
                MCC_LOG_WARN("glm::vec<{}> is not implemented for Program::SetUniformVector", L);
        }
    }

}