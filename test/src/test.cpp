
#include <string>

#include <catch2/catch.hpp>

#include <Parser.hpp>


namespace fs = std::filesystem;


TEST_CASE("0000: Check Line - Complex", "[Line]")
{
    const fs::path inputFile{"test_cases/0000.OLB"};

    Parser parser{inputFile};
    Library lib = parser.parseLibrary();

    const Line& line = lib.packages.at(0).geometrySpecifications.at(0).lines.at(0);

    REQUIRE(line.getLineStyle() == LineStyle::DashDot);
    REQUIRE(line.getLineWidth() == LineWidth::Wide);

    REQUIRE(line.x1 == 10);
    REQUIRE(line.y1 == 20);

    REQUIRE(line.x2 == 20);
    REQUIRE(line.y2 == 40);
}


TEST_CASE("0000: Check CommentText - Complex", "[CommentText]")
{
    const fs::path inputFile{"test_cases/0001.OLB"};

    Parser parser{inputFile};
    Library lib = parser.parseLibrary();

    const CommentText& text = lib.packages.at(0).geometrySpecifications.at(0).commentTexts.at(0);

    REQUIRE(text.locX == 28);
    REQUIRE(text.locY == 20);

    REQUIRE(text.x1 == 28);
    REQUIRE(text.y1 == 20);

    REQUIRE(text.x2 == 118);
    REQUIRE(text.y2 == 42);

    REQUIRE(text.name == "Example text\nwith line break!");

    const TextFont& font = text.getTextFont();

    REQUIRE(font.fontName == "Courier New");
    REQUIRE(font.width  ==  0);
    REQUIRE(font.height == -9);
    REQUIRE(font.escapement == 0);
    REQUIRE(font.italic == 0);
    REQUIRE(font.weight == 400);
}


TEST_CASE("0002: Check Rect - Complex", "[Rect]")
{
    const fs::path inputFile{"test_cases/0002.OLB"};

    Parser parser{inputFile};
    Library lib = parser.parseLibrary();

    const Rect& rect = lib.packages.at(0).geometrySpecifications.at(0).rects.at(0);

    REQUIRE(rect.fillStyle  == FillStyle::HatchPattern);
    REQUIRE(rect.hatchStyle == HatchStyle::Mesh);

    REQUIRE(rect.getLineStyle() == LineStyle::Dash);
    REQUIRE(rect.getLineWidth() == LineWidth::Thin);

    REQUIRE(rect.x1 == 20);
    REQUIRE(rect.y1 == 12);

    REQUIRE(rect.x2 == 30);
    REQUIRE(rect.y2 == 62);
}


TEST_CASE("0003: Check Ellipse - Complex", "[Ellipse]")
{
    const fs::path inputFile{"test_cases/0003.OLB"};

    Parser parser{inputFile};
    Library lib = parser.parseLibrary();

    const Ellipse& ellipse = lib.packages.at(0).geometrySpecifications.at(0).ellipses.at(0);

    REQUIRE(ellipse.getFillStyle()  == FillStyle::Solid);
    REQUIRE(ellipse.getHatchStyle() == HatchStyle::NotValid);

    REQUIRE(ellipse.getLineStyle() == LineStyle::Default);
    REQUIRE(ellipse.getLineWidth() == LineWidth::Thin);

    REQUIRE(ellipse.x1 == 10);
    REQUIRE(ellipse.y1 == 20);

    REQUIRE(ellipse.x2 == 40);
    REQUIRE(ellipse.y2 == 30);
}


TEST_CASE("0004: Check Arc - Complex", "[Arc]")
{
    const fs::path inputFile{"test_cases/0004.OLB"};

    Parser parser{inputFile};
    Library lib = parser.parseLibrary();

    const Arc& arc = lib.packages.at(0).geometrySpecifications.at(0).arcs.at(0);

    REQUIRE(arc.getLineStyle() == LineStyle::Dash);
    REQUIRE(arc.getLineWidth() == LineWidth::Medium);

    REQUIRE(arc.x1 ==  0);
    REQUIRE(arc.y1 == 20);

    REQUIRE(arc.x2 == 120);
    REQUIRE(arc.y2 ==  40);

    REQUIRE(arc.startX == 60);
    REQUIRE(arc.startY == 20);

    REQUIRE(arc.endX == 120);
    REQUIRE(arc.endY ==  30);
}


TEST_CASE("0005: Check Bezier - Complex", "[Bezier]")
{
    const fs::path inputFile{"test_cases/0005.OLB"};

    Parser parser{inputFile};
    Library lib = parser.parseLibrary();

    const Bezier& bezier = lib.packages.at(0).geometrySpecifications.at(0).beziers.at(0);

    REQUIRE(bezier.getLineStyle() == LineStyle::Solid);
    REQUIRE(bezier.getLineWidth() == LineWidth::Thin);

    REQUIRE(bezier.points.at(0).x == 10);
    REQUIRE(bezier.points.at(0).y == 10);

    REQUIRE(bezier.points.at(1).x == 30);
    REQUIRE(bezier.points.at(1).y == 20);

    REQUIRE(bezier.points.at(2).x == 40);
    REQUIRE(bezier.points.at(2).y == 10);

    REQUIRE(bezier.points.at(3).x == 60);
    REQUIRE(bezier.points.at(3).y ==  0);

    REQUIRE(bezier.points.at(4).x == 70);
    REQUIRE(bezier.points.at(4).y == 20);

    REQUIRE(bezier.points.at(5).x == 60);
    REQUIRE(bezier.points.at(5).y == 30);

    REQUIRE(bezier.points.at(6).x == 30);
    REQUIRE(bezier.points.at(6).y == 20);

    REQUIRE(bezier.points.at(7).x == 10);
    REQUIRE(bezier.points.at(7).y == 20);

    REQUIRE(bezier.points.at(8).x ==  0);
    REQUIRE(bezier.points.at(8).y == 30);

    REQUIRE(bezier.points.at(9).x ==  0);
    REQUIRE(bezier.points.at(9).y == 50);

    REQUIRE(bezier.points.at(10).x == 30);
    REQUIRE(bezier.points.at(10).y == 40);

    REQUIRE(bezier.points.at(11).x == 30);
    REQUIRE(bezier.points.at(11).y == 40);

    REQUIRE(bezier.points.at(12).x == 30);
    REQUIRE(bezier.points.at(12).y == 40);
}


TEST_CASE("0006: Check Polyline - Complex", "[Polyline]")
{
    const fs::path inputFile{"test_cases/0006.OLB"};

    Parser parser{inputFile};
    Library lib = parser.parseLibrary();

    const Polyline& polyline = lib.packages.at(0).geometrySpecifications.at(0).polylines.at(0);

    REQUIRE(polyline.getLineStyle() == LineStyle::Solid);
    REQUIRE(polyline.getLineWidth() == LineWidth::Thin);

    REQUIRE(polyline.points.at(0).x == 10);
    REQUIRE(polyline.points.at(0).y == 10);

    REQUIRE(polyline.points.at(1).x == 20);
    REQUIRE(polyline.points.at(1).y == 10);

    REQUIRE(polyline.points.at(2).x == 20);
    REQUIRE(polyline.points.at(2).y == 20);

    REQUIRE(polyline.points.at(3).x == 30);
    REQUIRE(polyline.points.at(3).y == 20);
}


TEST_CASE("0007: Check Bitmap - Complex", "[Bitmap]")
{
    const fs::path inputFile{"test_cases/0007.OLB"};

    Parser parser{inputFile};
    Library lib = parser.parseLibrary();

    const Bitmap& bitmap = lib.packages.at(0).geometrySpecifications.at(0).bitmaps.at(0);

    REQUIRE(bitmap.locX == 20);
    REQUIRE(bitmap.locY == 10);

    REQUIRE(bitmap.x1 == 20);
    REQUIRE(bitmap.y1 == 10);

    REQUIRE(bitmap.x2 == 40);
    REQUIRE(bitmap.y2 == 50);

    const std::vector<uint8_t> expectedRawData = {
            0x28, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
            0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0xEB, 0xEB, 0xEB, 0x00, 0x00, 0xEB, 0x00, 0xEB,
            0x00, 0xEB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        };

    REQUIRE(bitmap.rawImgData == expectedRawData);
}