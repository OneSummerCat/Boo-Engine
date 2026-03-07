#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <cstdint>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

namespace Boo {

struct SDFStruct
{
    int dx;
    int dy;
    float dis;
    float sdfValue;
}; 

struct Point {
    int x, y;
    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}
};
class SDFUtil {
private:
    static void _initialize(const std::vector<uint8_t> &pixels,int width, int height,std::vector<SDFStruct> &sdfStructs);
    static void _updatePixel(std::vector<SDFStruct> &sdfStructs,int width, int height,int x,int y,int nx,int ny);
    static void _logSDF(const std::vector<SDFStruct> &sdfStructs, int width, int height);
    static void _logSDF(const std::vector<uint8_t> &pixels, int width, int height);
    
public:
    static std::vector<uint8_t> generateSDF(const std::vector<uint8_t> &pixels, int width, int height);
};

} // namespace Boo