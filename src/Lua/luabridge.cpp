#include "luabridge.h"

// \todo PermanentStorage should be used instead.
extern SdFs SD;
bool lua_windowRepaintEnabled;

// \todo temp fix. Lua needs its own class.
#include "System.h"
lua_State *L = System::L;

// Compatibility functions for Serial output and SdFat file operations
extern "C" {

StdioStream lua_temp_stdio_file;

int _gettimeofday(struct timeval *tv, void *tzvp __attribute__((unused)))
{
    return (0);
}

clock_t _times(struct tms *buffer)
{
    return (0);
}

int lua_compat_fopen(const char *filename)
{
    // Serial << "-- sd fopen: " << filename << "\n";
    return (lua_temp_stdio_file.fopen(filename, "r") ? 1 : 0);
}

void lua_compat_fclose()
{
    // Serial << "-- sd fclose\n";
    lua_temp_stdio_file.fclose();
}

char lua_compat_getc()
{
    // Serial << "-- sd getc\n";
    return (lua_temp_stdio_file.getc());
}

int lua_compat_feof()
{
    // Serial << "-- sd feof\n";
    return (lua_temp_stdio_file.feof());
}

size_t lua_compat_fread(void *ptr, size_t size, size_t count)
{
    // Serial << "-- sd fread -- size: " << size << " count: " << count << "\n";
    return (lua_temp_stdio_file.fread(ptr, size, count));
}

int lua_compat_ferror()
{
    // Serial << "-- sd ferror\n";
    return (lua_temp_stdio_file.ferror());
}

void lua_compat_printf(char *format, ...)
{
    char buf[256];
    va_list ap;

    va_start(ap, format);
    logMessage(buf, sizeof(buf), format, ap);
    va_end(ap);
}

void lua_compat_print(const char *s)
{
    if (s && (s[0] != '\n') && (s[0] != '\0')) {
        logMessage("lua: %s", s);
    }
}

} // extern C

static const luaL_Reg eoslibs[] = { { "midi", luaopen_midi },
                                    { "timer", luaopen_timer },
                                    { "transport", luaopen_transport },
                                    { "window", luaopen_window },
                                    { NULL, NULL } };

void initLua(void)
{
    L = nullptr;
    // Enable printf/sprintf to print floats
    asm(".global _printf_float");

    L = luaL_newstate(); /* opens Lua */
    luaL_openlibs(L); /* opens the standard libraries */
    luaLE_openEoslibs(L, eoslibs);

    // register userdate objects
    sysexBlock_register(L);

    // register global variables
    loadGlobalVariables(L);
}

void closeLua(void)
{
    if (L) {
        timer_disable(L);
        transport_disable(L);
        lua_close(L);
        L = nullptr;
    }
}

void loadLuaModule(const char *filename)
{
    // Clear the stack
    lua_settop(L, 0);

    int error = luaL_dofile(L, filename);

    if (error) {
        logMessage("loadLuaModule: error loading file: filename=%s, error=%s",
                   filename,
                   lua_tostring(L, -1));
        lua_pop(L, 1); /* pop error message from the stack */
    } else {
        logMessage("loadLuaModule: Lua extension file initialized: file=%s",
                   filename);
    }
}

void runLuaFunction(const char *functionName)
{
    lua_getglobal(L, functionName);

    if (lua_isfunction(L, -1)) {
        if (lua_pcall(L, 0, 0, 0) != 0) {
            logMessage("error running function '%s': %s",
                       functionName,
                       lua_tostring(L, -1));
        }
    }
}

void runLuaString(const char *commandText)
{
    lua_settop(L, 0);

    int error = luaL_dostring(L, commandText);

    if (error) {
        logMessage("runLuaString: error loading file: error=%s",
                   lua_tostring(L, -1));
        lua_pop(L, 1); /* pop error message from the stack */
    }
}

void executeElectraLua(const char *filename)
{
    lua_windowRepaintEnabled = false;
    logMessage("loadLua debug output:");
    logMessage("---- START ----");
    loadLuaModule(filename);
    logMessage("----- END -----");
    lua_windowRepaintEnabled = true;
}

bool isLuaValid(const char *filename)
{
    size_t filesize = 0;
    File file = Hardware::sdcard.createInputStream(filename);

    if (file) {
        // \todo do the Lua source validation here
        filesize = file.fileSize();
        file.close();
    } else {
        return (false);
    }

    return (true);
}

void loadGlobalVariables(lua_State *L)
{
    // colours
    lua_pushnumber(L, 0xffffff);
    lua_setglobal(L, "WHITE");
    lua_pushnumber(L, 0xF45C51);
    lua_setglobal(L, "RED");
    lua_pushnumber(L, 0xF49500);
    lua_setglobal(L, "ORANGE");
    lua_pushnumber(L, 0x529DEC);
    lua_setglobal(L, "BLUE");
    lua_pushnumber(L, 0x03A598);
    lua_setglobal(L, "GREEN");
    lua_pushnumber(L, 0xC44795);
    lua_setglobal(L, "PURPLE");

    // midi interfaces
    lua_pushnumber(L, 0);
    lua_setglobal(L, "MIDIIO");
    lua_pushnumber(L, 1);
    lua_setglobal(L, "USBDEV");
    lua_pushnumber(L, 2);
    lua_setglobal(L, "USBHOST");

    // midi ports
    lua_pushnumber(L, 0);
    lua_setglobal(L, "PORT_1");
    lua_pushnumber(L, 1);
    lua_setglobal(L, "PORT_2");
    lua_pushnumber(L, 2);
    lua_setglobal(L, "PORT_CTRL");

    // Change origins
    lua_pushnumber(L, 0);
    lua_setglobal(L, "INTERNAL");
    lua_pushnumber(L, 1);
    lua_setglobal(L, "MIDI");
    lua_pushnumber(L, 2);
    lua_setglobal(L, "LUA");

    // hardware buttons
    lua_pushnumber(L, 1);
    lua_setglobal(L, "BUTTON_1");
    lua_pushnumber(L, 2);
    lua_setglobal(L, "BUTTON_2");
    lua_pushnumber(L, 3);
    lua_setglobal(L, "BUTTON_3");
    lua_pushnumber(L, 4);
    lua_setglobal(L, "BUTTON_4");
    lua_pushnumber(L, 5);
    lua_setglobal(L, "BUTTON_5");
    lua_pushnumber(L, 6);
    lua_setglobal(L, "BUTTON_6");

    // pots
    lua_pushnumber(L, 1);
    lua_setglobal(L, "POT_1");
    lua_pushnumber(L, 2);
    lua_setglobal(L, "POT_2");
    lua_pushnumber(L, 3);
    lua_setglobal(L, "POT_3");
    lua_pushnumber(L, 4);
    lua_setglobal(L, "POT_4");
    lua_pushnumber(L, 5);
    lua_setglobal(L, "POT_5");
    lua_pushnumber(L, 6);
    lua_setglobal(L, "POT_6");
    lua_pushnumber(L, 7);
    lua_setglobal(L, "POT_7");
    lua_pushnumber(L, 8);
    lua_setglobal(L, "POT_8");
    lua_pushnumber(L, 9);
    lua_setglobal(L, "POT_9");
    lua_pushnumber(L, 10);
    lua_setglobal(L, "POT_10");
    lua_pushnumber(L, 11);
    lua_setglobal(L, "POT_11");
    lua_pushnumber(L, 12);
    lua_setglobal(L, "POT_12");

    // touch points
    lua_pushnumber(L, 1);
    lua_setglobal(L, "TOUCH_POINT_1");
    lua_pushnumber(L, 2);
    lua_setglobal(L, "TOUCH_POINT_2");
    lua_pushnumber(L, 3);
    lua_setglobal(L, "TOUCH_POINT_3");
    lua_pushnumber(L, 4);
    lua_setglobal(L, "TOUCH_POINT_4");
    lua_pushnumber(L, 5);
    lua_setglobal(L, "TOUCH_POINT_5");

    // MIDI message types
    lua_pushnumber(L, 176);
    lua_setglobal(L, "CONTROL_CHANGE");
    lua_pushnumber(L, 144);
    lua_setglobal(L, "NOTE_ON");
    lua_pushnumber(L, 128);
    lua_setglobal(L, "NOTE_OFF");
    lua_pushnumber(L, 192);
    lua_setglobal(L, "PROGRAM_CHANGE");
    lua_pushnumber(L, 160);
    lua_setglobal(L, "POLY_PRESSURE");
    lua_pushnumber(L, 208);
    lua_setglobal(L, "CHANNEL_PRESSURE");
    lua_pushnumber(L, 224);
    lua_setglobal(L, "PITCH_BEND");
    lua_pushnumber(L, 248);
    lua_setglobal(L, "CLOCK");
    lua_pushnumber(L, 250);
    lua_setglobal(L, "START");
    lua_pushnumber(L, 252);
    lua_setglobal(L, "STOP");
    lua_pushnumber(L, 251);
    lua_setglobal(L, "CONTINUE");
    lua_pushnumber(L, 254);
    lua_setglobal(L, "ACTIVE_SENSING");
    lua_pushnumber(L, 255);
    lua_setglobal(L, "RESET");
    lua_pushnumber(L, 243);
    lua_setglobal(L, "SONG_SELECT");
    lua_pushnumber(L, 242);
    lua_setglobal(L, "SONG_POSITION");
    lua_pushnumber(L, 246);
    lua_setglobal(L, "TUNE_REQUEST");
    lua_pushnumber(L, 241);
    lua_setglobal(L, "TIME_CODE_QUARTER_FRAME");
    lua_pushnumber(L, 240);
    lua_setglobal(L, "SYSEX");

    // Electra Parameter types
    lua_pushnumber(L, 0);
    lua_setglobal(L, "PT_VIRTUAL");
    lua_pushnumber(L, 1);
    lua_setglobal(L, "PT_CC7");
    lua_pushnumber(L, 2);
    lua_setglobal(L, "PT_CC14");
    lua_pushnumber(L, 3);
    lua_setglobal(L, "PT_NRPN");
    lua_pushnumber(L, 4);
    lua_setglobal(L, "PT_RPN");
    lua_pushnumber(L, 5);
    lua_setglobal(L, "PT_NOTE");
    lua_pushnumber(L, 6);
    lua_setglobal(L, "PT_PROGRAM");
    lua_pushnumber(L, 7);
    lua_setglobal(L, "PT_SYSEX");
    lua_pushnumber(L, 8);
    lua_setglobal(L, "PT_START");
    lua_pushnumber(L, 9);
    lua_setglobal(L, "PT_STOP");
    lua_pushnumber(L, 10);
    lua_setglobal(L, "PT_TUNE");
    lua_pushnumber(L, 11);
    lua_setglobal(L, "PT_UNKNOWN");

    // MIDI interfaces
    lua_pushnumber(L, 0);
    lua_setglobal(L, "MIDI_IO");
    lua_pushnumber(L, 1);
    lua_setglobal(L, "USB_DEV");
    lua_pushnumber(L, 2);
    lua_setglobal(L, "USB_HOST");

    lua_pushnumber(L, 1);
    lua_setglobal(L, "X");
    lua_pushnumber(L, 2);
    lua_setglobal(L, "Y");
    lua_pushnumber(L, 3);
    lua_setglobal(L, "WIDTH");
    lua_pushnumber(L, 4);
    lua_setglobal(L, "HEIGHT");

    lua_pushnumber(L, 1);
    lua_setglobal(L, "CONTROL_SET_1");
    lua_pushnumber(L, 2);
    lua_setglobal(L, "CONTROL_SET_2");
    lua_pushnumber(L, 3);
    lua_setglobal(L, "CONTROL_SET_3");
}
