#pragma once

#include <iostream>
#include <string>

#include <vector>
#include <SDL.h>

/*! \class defines
    \brief A header that contains structures and enumerators
    They are used for the different type of game states, ores and more
*/
using namespace std;
///A user-defined function similar to std::cout but giving more information
/**
*   @code
*   #define D(x) cerr << #x << " = " << (x) << " | " << __LINE__ << endl;
*   @endcode
*/
///A user-defined function similar to D(x) but more advanced
/**
*   @code
*   #define Dex(x) cerr << #x << " = " << (x) << " | " << __LINE__ << " " << __FILE__ << endl;
*   @endcode
*/
#define D(x) cerr << #x << " = " << (x) << " | " << __LINE__ << endl;
#define Dex(x) cerr << #x << " = " << (x) << " | " << __LINE__ << " " << __FILE__ << endl;
#define __FILE_NAME__ (strrchr(__FILE__, '\\' ) ? strrchr(__FILE__, '\\') + 1 : __FILE__)

static string UI_FOLDER = "UI\\";
static string MENU_FOLDER = "menu\\";
static string MAIN_FOLDER = "main\\";
static string IMG_FOLDER = "img\\";
static string CONFIG_FOLDER = "config\\";
static string FONT_FOLDER = "ttf\\";
static string DATABASE_FOLDER = "database\\";
static string VFX_FOLDER = "vfx\\";
static string IN_GAME_BUTTONS_FOLDER = "InGameButtons\\";
static string CONFIG_MANAGER_FOLDER = "configManager\\";
static string CLIENT_FOLDER = "client\\";

struct int2
{
    int x = 0;
    int y = 0;

    void reset()
    {
        x = 0;
        y = 0;
    }

    template <typename T>
    void operator*=(T a)
    {
        x *= a;
        y *= a;
    }

    template <typename T>
    void operator/=(T a)
    {
        x /= a;
        y /= a;
    }

    template <typename T>
    int2 operator*(T a)
    {
        x *= a;
        y *= a;

        return *this;
    }

    template <typename T>
    int2 operator/(T a)
    {
        x /= a;
        y /= a;

        return *this;
    }

    // used in the animator
    template <typename T>
    bool operator!=(T a)
    {
        return (x != a || y != a);
    }
};

struct float2
{
    float x = 0;
    float y = 0;

    void reset()
    {
        x = 0;
        y = 0;
    }

    void operator=(int2 a)
    {
        x = a.x;
        y = a.y;
    }

    void operator+=(float2 a)
    {
        x += a.x;
        y += a.y;
    }

    void operator-=(float2 a)
    {
        x -= a.x;
        y -= a.y;
    }

    template <typename T>
    void operator*=(T a)
    {
        x *= a;
        y *= a;
    }

    template <typename T>
    void operator/=(T a)
    {
        x /= a;
        y /= a;
    }

    template <typename T>
    float2 operator*(T a)
    {
        x *= a;
        y *= a;

        return *this;
    }

    template <typename T>
    float2 operator/(T a)
    {
        x /= a;
        y /= a;

        return *this;
    }

    template <typename T>
    bool operator!=(T a)
    {
        return (x != a || y != a);
    }

    template <typename T>
    bool operator>(T a)
    {
        return (x > a || y > a);
    }

    template <typename T>
    bool operator<(T a)
    {
        return (x < a || y < a);
    }

    template <typename T>
    bool operator==(T a)
    {
        return (x == a && y == a);
    }
};

static int2 parseToCoordinates(SDL_Rect rect)
{
    return { rect.x , rect.y };
}

struct line
{
    float2 start;
    float2 finish;
}; ///< A useful structure creating a line from coordinates

enum class FONT
{
    NONE = 0,
    ARCADE_CLASSIC = 1, // used in MonTu
    ADVENT_PRO = 2 // used in Raven
};

enum class COLOR
{
    NONE = 0,
    LIGHT = 1,
    DARK = 2
};

struct Drawable
{
    SDL_Texture* texture = nullptr;
    SDL_Rect rect = { 0 }; /// The rect where we draw
};