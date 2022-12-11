#include "Tutor.hpp"
#include "Common.hpp"
#include <glm/glm.hpp>

int SampleMain()
{
    Tutor app;
    app.Create();
    app.Run();

    return 0;
}

Tutor::Tutor()
    : App3DFramework("Tutor1")
{
}