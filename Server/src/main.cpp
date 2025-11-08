// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#include "Server/ServerApplication.h"

int main(const int argc, char** argv)
{
    Mcc::ServerApplication app(argc, argv);
    return app.Run();
}