#include "common/luax.h"
#include "modules/joystick/wrap_joystick.h"
#include "objects/gamepad/wrap_gamepad.h"

using namespace love;

#define instance() (Module::GetInstance<Joystick>(Module::M_JOYSTICK))

int Wrap_Joystick::GetJoystickCount(lua_State * L)
{
    size_t count = instance()->GetJoystickCount();

    lua_pushnumber(L, count);

    return 1;
}

int Wrap_Joystick::GetJoysticks(lua_State * L)
{
    size_t size = instance()->GetJoystickCount();

    lua_createtable(L, size, 0);

    for (size_t i = 0; i < size; i++)
    {
        Gamepad * gamepad = instance()->GetJoystickFromID(i);

        Luax::PushType(L, gamepad);
        lua_rawseti(L, -2, i + 1);
    }

    return 1;
}

Gamepad * Wrap_Joystick::CheckGamepad(lua_State * L, int index)
{
    return Luax::CheckType<Gamepad>(L, index);
}

int Wrap_Joystick::Register(lua_State * L)
{
    luaL_Reg reg[] =
    {
        { "getJoystickCount",   GetJoystickCount },
        { "getJoysticks",       GetJoysticks     },
        { 0,                    0                }
    };

    lua_CFunction types[] =
    {
        Wrap_Gamepad::Register,
        0
    };

    Joystick * instance = instance();
    if (instance == nullptr)
        Luax::CatchException(L, [&]() { instance = new Joystick(); });
    else
        instance->Retain();

    WrappedModule wrappedModule;

    wrappedModule.instance = instance;
    wrappedModule.name = "joystick";
    wrappedModule.functions = reg;
    wrappedModule.type = &Module::type;
    wrappedModule.types = types;

    return Luax::RegisterModule(L, wrappedModule);
}
