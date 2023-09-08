#pragma once

#include <string>

struct Color {
    inline Color(float r, float g, float b, float a = 1)
        : r(r), g(g), b(b), a(a) {}

    inline std::string Format() const {
        char* buffer;

        asprintf(&buffer, "rgba(%.2f, %.2f, %.2f, %.2f)", r, g, b, a);

        return buffer;
    }

    static const Color& Black() {
        static const Color color{0, 0, 0};
        return color;
    }

    static const Color& White() {
        static const Color color{255, 255, 255};
        return color;
    }

    static const Color& Red() {
        static const Color color{255, 0, 0};
        return color;
    }

    static const Color& Green() {
        static const Color color{0, 255, 0};
        return color;
    }

    static const Color& Blue() {
        static const Color color{0, 0, 255};
        return color;
    }

    float r, g, b, a;
};

struct Stroke {
    inline std::string Format() const {
        const char* str =
            width != 0 ? "stroke: %s; stroke-width: %.2f" : "stroke: none";

        if (width != 0) {
            char* buffer;

            asprintf(&buffer, str, color.Format().c_str(), width);

            return buffer;
        }

        return str;
    }

    Color color = Color::Black();
    float width = 0;
};

struct Style {
    inline Style() : fill(Color::White()) {}

    inline std::string Format() const {
        char* buffer;

        asprintf(&buffer, "%s; fill: %s; fill-opacity: %.2f",
                 stroke.Format().c_str(), fill.Format().c_str(), opacity);

        return buffer;
    }

    Color fill;
    Stroke stroke;
    float opacity;
};

namespace SVG {
inline std::string Begin(float w, float h) {
    char* buffer;

    asprintf(
        &buffer,
        R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 %.2f %.2f">)",
        w, h);

    return buffer;
}

inline std::string End() {
    return "</svg>";
}
};  // namespace SVG

class Rectangle {
   public:
    inline Rectangle(float x, float y, float width, float height)
        : x(x), y(y), width(width), height(height) {}

    inline Rectangle& WithFill(const Color& fill) {
        style.fill = fill;
        return *this;
    }

    inline Rectangle& WithStroke(const Stroke& stroke) {
        style.stroke = stroke;
        return *this;
    }

    inline Rectangle& WithOpacity(float opacity) {
        style.opacity = opacity;
        return *this;
    }

    inline Rectangle& WithBorderRadius(float radius) {
        borderRadius = radius;
        return *this;
    }

    inline Rectangle& Offset(float x, float y) {
        this->x += x;
        this->y += y;
        return *this;
    }

    inline Rectangle& Inflate(float width, float height) {
        x -= width;
        y -= height;
        this->width += 2.0 * width;
        this->width += 2.0 * height;
        return *this;
    }

    inline std::string Format() {
        char* buffer;

        asprintf(
            &buffer,
            R"(    <rect x="%.2f" y="%.2f" width="%.2f" height="%.2f" ry="%.2f" style="%s" />)",
            x, y, width, height, borderRadius, style.Format().c_str());

        return buffer;
    }

   private:
    Style style;
    float x, y, width, height;
    float borderRadius = 0;
};

class Text {
   public:
    enum Align { LEFT, RIGHT, CENTER };

    inline Text(float x, float y, const std::string& content)
        : x(x), y(y), content(content) {}

    inline Text& WithColor(const Color& color) {
        this->color = color;
        return *this;
    }

    inline Text& WithSize(float size) {
        this->size = size;
        return *this;
    }

    inline Text& WithAlign(const Align& align) {
        this->align = align;
        return *this;
    }

    inline Text& Offset(float x, float y) {
        this->x += x;
        this->y += y;
        return *this;
    }

    inline std::string Format() {
        char* buffer;
        std::string alignment;

        switch (align) {
            case LEFT: {
                alignment = "text-anchor: start; text-align: left;";
                break;
            }
            case RIGHT: {
                alignment = "text-anchor: end; text-align: right;";
                break;
            }
            case CENTER: {
                alignment = "text-anchor: middle; text-align: center;";
                break;
            }
        }

        asprintf(
            &buffer,
            R"(    <text x="%.2f" y="%.2f" style="font-size: %.2fpx; fill: %s; %s">%s</text>)",
            x, y, size, color.Format().c_str(), alignment.c_str(),
            content.c_str());

        return buffer;
    }

   private:
    float x, y;
    float size = 10;
    std::string content;
    Color color = Color::Black();
    Align align = Align::LEFT;
};

class LineSegment {
   public:
    inline LineSegment(float x1, float x2, float y1, float y2)
        : x1(x1), x2(x2), y1(y1), y2(y2) {}

    inline LineSegment& WithColor(const Color& color) {
        this->color = color;
        return *this;
    }

    inline LineSegment& WithWidth(float width) {
        this->width = width;
        return *this;
    }

    inline LineSegment& Offset(float x, float y) {
        this->x1 += x;
        this->x2 += x;
        this->y1 += y;
        this->y2 += y;
        return *this;
    }

    inline std::string Format() {
        char* buffer;

        asprintf(
            &buffer,
            R"(    <path d="M %.2f %.2f L %.2f %.2f" style="stroke: %s; stroke-width: %.2f"/>")",
            x1, x2, y1, y2, color.Format().c_str(), width);

        return buffer;
    }

   private:
    float x1, x2, y1, y2;
    float width = 1;
    Color color = Color::Black();
};