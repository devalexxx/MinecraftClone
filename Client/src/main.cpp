// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#include "Client/ClientApplication.h"

int main(const int argc, char** argv)
{
    Mcc::ClientApplication app(argc, argv);
    return app.Run();
}