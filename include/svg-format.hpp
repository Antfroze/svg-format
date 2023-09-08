#pragma once

#include <string>
#include <vector>

namespace svg_fmt {
struct Color {
    inline Color(float r, float g, float b, float a = 1)
        : r(r), g(g), b(b), a(a) {}

    inline std::string Format() const {
        char* buffer;

        if (a != 1) {
            asprintf(&buffer, "rgba(%.3g, %.3g, %.3g, %.3g)", r, g, b, a);

        } else {
            asprintf(&buffer, "rgb(%.3g, %.3g, %.3g)", r, g, b);
        }

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
        std::string str =
            width != 0 ? "stroke: %s; stroke-width: %.3g" : "stroke: none";

        if (width != 0) {
            char* buffer;

            asprintf(&buffer, str.c_str(), color.Format().c_str(), width);

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

        asprintf(&buffer, "%s; fill: %s; fill-opacity: %.3g",
                 stroke.Format().c_str(), fill.Format().c_str(), opacity);

        return buffer;
    }

    Color fill;
    Stroke stroke;
    float opacity = 0;
};

namespace SVG {
inline std::string Begin(float w, float h) {
    char* buffer;

    asprintf(
        &buffer,
        R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 %.3g %.3g">)",
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
        this->width += width * 2;
        this->width += height * 2;
        return *this;
    }

    inline std::string Format() const {
        char* buffer;

        asprintf(
            &buffer,
            R"(    <rect x="%.3g" y="%.3g" width="%.3g" height="%.3g" ry="%.3g" style="%s" />)",
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

    inline std::string Format() const {
        char* buffer;
        std::string alignment;

        switch (align) {
            case LEFT: {
                alignment = "text-anchor: start; text-align: left";
                break;
            }
            case RIGHT: {
                alignment = "text-anchor: end; text-align: right";
                break;
            }
            case CENTER: {
                alignment = "text-anchor: middle; text-align: center";
                break;
            }
        }

        asprintf(
            &buffer,
            R"(    <text x="%.3g" y="%.3g" style="font-size: %.3gpx; fill: %s; %s">%s</text>)",
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

    inline std::string Format() const {
        char* buffer;

        asprintf(
            &buffer,
            R"(    <path d="M %.3g %.3g L %.3g %.3g" style="stroke: %s; stroke-width: %.3g" />")",
            x1, x2, y1, y2, color.Format().c_str(), width);

        return buffer;
    }

   private:
    float x1, x2, y1, y2;
    float width = 1;
    Color color = Color::Black();
};

class Polygon {
   public:
    inline Polygon(const std::vector<std::array<float, 2>>& points)
        : points(points) {}

    inline Polygon& Open() {
        closed = false;
        return *this;
    }

    inline Polygon& WithFill(const Color& color) {
        this->style.fill = color;
        return *this;
    }

    inline Polygon& WithStroke(const Stroke& stroke) {
        this->style.stroke = stroke;
        return *this;
    }

    inline Polygon& WithOpacity(float opacity) {
        this->style.opacity = opacity;
        return *this;
    }

    inline std::string Format() const {
        char* buffer = nullptr;

        asprintf(&buffer, R"(    <path d=")");

        if (!points.empty()) {
            strcat(buffer, "M ");

            char temp[100];
            for (size_t i = 0; i < points.size(); ++i) {
                snprintf(temp, sizeof(temp), "%.3g %.3g", points[i][0],
                         points[i][1]);
                strcat(buffer, temp);

                if (i != points.size() - 1) {
                    strcat(buffer, " ");
                }
            }

            if (closed) {
                strcat(buffer, " Z");
            }
        }

        strcat(buffer, R"(" style=")");
        strcat(buffer, style.Format().c_str());
        strcat(buffer, R"(" />)");

        return buffer;
    }

   private:
    std::vector<std::array<float, 2>> points;
    bool closed = true;
    Style style;
};

struct Triangle : public Polygon {
    inline Triangle(float x1, float x2, float x3, float y1, float y2, float y3)
        : Polygon({{x1, y1}, {x2, y2}, {x3, y3}}) {}
};

class Circle {
   public:
    inline Circle(float x, float y, float radius)
        : x(x), y(y), radius(radius) {}

    inline Circle& WithFill(const Color& fill) {
        this->style.fill = fill;
        return *this;
    }

    inline Circle& WithStroke(const Stroke& stroke) {
        this->style.stroke = stroke;
        return *this;
    }

    inline Circle& WithOpacity(float opacity) {
        this->style.opacity = opacity;
        return *this;
    }

    inline Circle& Offset(float x, float y) {
        this->x += x;
        this->y += y;
        return *this;
    }

    inline Circle& Inflate(float radius) {
        x -= radius;
        y -= radius;
        this->radius += radius * 2;
        return *this;
    }

    inline std::string Format() const {
        char* buffer;

        asprintf(&buffer,
                 R"(    <circle cx="%.3g" cy="%.3g" r="%.3g" style="%s" />)", x,
                 y, radius, style.Format().c_str());

        return buffer;
    }

   private:
    float x, y, radius;
    Style style;
};

class Path {};
}  // namespace svg_fmt
