
#include "System/Drawing/Drawing.h"
#include <iostream>
#include <memory>

using namespace System::Drawing;

// Forward declaration of helper function
std::unique_ptr<Graphics> CreateGraphicsFromSkSurface(int width, int height);

int main() {
    try {
        std::cout << "System.Drawing Basic Example" << std::endl;
        
        // Test basic color operations
        Color red = Color::Red;
        Color blue = Color::Blue;
        Color custom = Color::FromArgb(128, 255, 128, 64);
        
        std::cout << "Red color: " << red.ToString() << std::endl;
        std::cout << "Blue color: " << blue.ToString() << std::endl;
        std::cout << "Custom color: " << custom.ToString() << std::endl;
        
        // Test geometric structures
        Point point(10, 20);
        Size size(100, 200);
        Rectangle rect(point, size);
        
        std::cout << "Point: " << point.ToString() << std::endl;
        std::cout << "Size: " << size.ToString() << std::endl;
        std::cout << "Rectangle: " << rect.ToString() << std::endl;
        
        // Test drawing tools
        Pen pen(red, 2.0f);
        SolidBrush brush(blue);
        
        std::cout << "Pen color: " << pen.GetColor().ToString() << std::endl;
        std::cout << "Pen width: " << pen.GetWidth() << std::endl;
        std::cout << "Brush color: " << brush.GetColor().ToString() << std::endl;
        
        // Test font
        FontFamily fontFamily("Arial");
        Font font(fontFamily, 12.0f, FontStyle::Bold);
        
        std::cout << "Font: " << font.ToString() << std::endl;
        
        // Test matrix operations
        Drawing2D::Matrix matrix;
        matrix.Translate(10, 20);
        matrix.Scale(2.0f, 2.0f);
        matrix.Rotate(45.0f);
        
        std::cout << "Matrix: " << matrix.ToString() << std::endl;
        
        // Test image format information
        auto decoders = Imaging::ImageCodecInfoHelper::GetImageDecoders();
        std::cout << "Available image decoders: " << decoders.size() << std::endl;
        for (const auto& decoder : decoders) {
            std::cout << "  - " << decoder.ToString() << std::endl;
        }
        
        std::cout << "System.Drawing example completed successfully!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
