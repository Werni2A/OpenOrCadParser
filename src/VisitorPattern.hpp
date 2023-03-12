#ifndef VISITORPATTERN_HPP
#define VISITORPATTERN_HPP


class Library;
class PageSettings;
class Point;
class PrimArc;
class PrimBezier;
class PrimBitmap;
class PrimCommentText;
class PrimEllipse;
class PrimLine;
class PrimPolygon;
class PrimPolyline;
class PrimRect;
class PrimSymbolVector;
class StreamAdminData;
class StreamCellsDirectory;
class StreamDsnStream;
class StreamERC;
class StreamExportBlocksDirectory;
class StreamGraphicsDirectory;
class StreamHierarchy;
class StreamHSObjects;
class StreamLibrary;
class StreamNetBundleMapData;
class StreamPackage;
class StreamPackagesDirectory;
class StreamPage;
class StreamPartsDirectory;
class StreamSchematic;
class StreamSymbol;
class StreamSymbolsDirectory;
class StreamType;
class StreamViewsDirectory;
class StructAlias;
class StructBookMarkSymbolInst;
class StructBusEntry;
class StructERCSymbol;
class StructERCSymbolInst;
class StructGeneralProperties;
class StructGlobal;
class StructGlobalSymbol;
class StructGraphicArcInst;
class StructGraphicBezierInst;
class StructGraphicBitMapInst;
class StructGraphicBoxInst;
class StructGraphicCommentTextInst;
class StructGraphicEllipseInst;
class StructGraphicLineInst;
class StructGraphicOleEmbedInst;
class StructGraphicPolygonInst;
class StructGraphicPolylineInst;
class StructHierarchicSymbol;
class StructOffPageConnector;
class StructOffPageSymbol;
class StructPartInst;
class StructPinIdxMapping;
class StructPinShapeSymbol;
class StructPort;
class StructPrimitives;
class StructProperties;
class StructSthInHierarchy1;
class StructSthInHierarchy2;
class StructSthInPages0;
class StructSymbol;
class StructSymbolBBox;
class StructSymbolDisplayProp;
class StructSymbolPin;
class StructSymbolPinBus;
class StructSymbolPinScalar;
class StructT0x10;
class StructT0x1f;
class StructT0x34;
class StructT0x35;
class StructTitleBlock;
class StructTitleBlockSymbol;
class StructWire;
class StructWireBus;
class StructWireScalar;
class tagLOGFONTA;

class Visitor
{
public:
    virtual void visit(const Library& aObj) = 0;
    virtual void visit(const PageSettings& aObj) = 0;
    virtual void visit(const Point& aObj) = 0;
    virtual void visit(const PrimArc& aObj) = 0;
    virtual void visit(const PrimBezier& aObj) = 0;
    virtual void visit(const PrimBitmap& aObj) = 0;
    virtual void visit(const PrimCommentText& aObj) = 0;
    virtual void visit(const PrimEllipse& aObj) = 0;
    virtual void visit(const PrimLine& aObj) = 0;
    virtual void visit(const PrimPolygon& aObj) = 0;
    virtual void visit(const PrimPolyline& aObj) = 0;
    virtual void visit(const PrimRect& aObj) = 0;
    virtual void visit(const PrimSymbolVector& aObj) = 0;
    virtual void visit(const StreamAdminData& aObj) = 0;
    virtual void visit(const StreamCellsDirectory& aObj) = 0;
    virtual void visit(const StreamDsnStream& aObj) = 0;
    virtual void visit(const StreamERC& aObj) = 0;
    virtual void visit(const StreamExportBlocksDirectory& aObj) = 0;
    virtual void visit(const StreamGraphicsDirectory& aObj) = 0;
    virtual void visit(const StreamHierarchy& aObj) = 0;
    virtual void visit(const StreamHSObjects& aObj) = 0;
    virtual void visit(const StreamLibrary& aObj) = 0;
    virtual void visit(const StreamNetBundleMapData& aObj) = 0;
    virtual void visit(const StreamPackage& aObj) = 0;
    virtual void visit(const StreamPackagesDirectory& aObj) = 0;
    virtual void visit(const StreamPage& aObj) = 0;
    virtual void visit(const StreamPartsDirectory& aObj) = 0;
    virtual void visit(const StreamSchematic& aObj) = 0;
    virtual void visit(const StreamSymbol& aObj) = 0;
    virtual void visit(const StreamSymbolsDirectory& aObj) = 0;
    virtual void visit(const StreamType& aObj) = 0;
    virtual void visit(const StreamViewsDirectory& aObj) = 0;
    virtual void visit(const StructAlias& aObj) = 0;
    virtual void visit(const StructBookMarkSymbolInst& aObj) = 0;
    virtual void visit(const StructBusEntry& aObj) = 0;
    virtual void visit(const StructERCSymbol& aObj) = 0;
    virtual void visit(const StructERCSymbolInst& aObj) = 0;
    virtual void visit(const StructGeneralProperties& aObj) = 0;
    virtual void visit(const StructGlobal& aObj) = 0;
    virtual void visit(const StructGlobalSymbol& aObj) = 0;
    virtual void visit(const StructGraphicArcInst& aObj) = 0;
    virtual void visit(const StructGraphicBezierInst& aObj) = 0;
    virtual void visit(const StructGraphicBitMapInst& aObj) = 0;
    virtual void visit(const StructGraphicBoxInst& aObj) = 0;
    virtual void visit(const StructGraphicCommentTextInst& aObj) = 0;
    virtual void visit(const StructGraphicEllipseInst& aObj) = 0;
    virtual void visit(const StructGraphicLineInst& aObj) = 0;
    virtual void visit(const StructGraphicOleEmbedInst& aObj) = 0;
    virtual void visit(const StructGraphicPolygonInst& aObj) = 0;
    virtual void visit(const StructGraphicPolylineInst& aObj) = 0;
    virtual void visit(const StructHierarchicSymbol& aObj) = 0;
    virtual void visit(const StructOffPageConnector& aObj) = 0;
    virtual void visit(const StructOffPageSymbol& aObj) = 0;
    virtual void visit(const StructPartInst& aObj) = 0;
    virtual void visit(const StructPinIdxMapping& aObj) = 0;
    virtual void visit(const StructPinShapeSymbol& aObj) = 0;
    virtual void visit(const StructPort& aObj) = 0;
    virtual void visit(const StructPrimitives& aObj) = 0;
    virtual void visit(const StructProperties& aObj) = 0;
    virtual void visit(const StructSthInHierarchy1& aObj) = 0;
    virtual void visit(const StructSthInHierarchy2& aObj) = 0;
    virtual void visit(const StructSthInPages0& aObj) = 0;
    virtual void visit(const StructSymbol& aObj) = 0;
    virtual void visit(const StructSymbolBBox& aObj) = 0;
    virtual void visit(const StructSymbolDisplayProp& aObj) = 0;
    virtual void visit(const StructSymbolPin& aObj) = 0;
    virtual void visit(const StructSymbolPinBus& aObj) = 0;
    virtual void visit(const StructSymbolPinScalar& aObj) = 0;
    virtual void visit(const StructT0x10& aObj) = 0;
    virtual void visit(const StructT0x1f& aObj) = 0;
    virtual void visit(const StructT0x34& aObj) = 0;
    virtual void visit(const StructT0x35& aObj) = 0;
    virtual void visit(const StructTitleBlock& aObj) = 0;
    virtual void visit(const StructTitleBlockSymbol& aObj) = 0;
    virtual void visit(const StructWire& aObj) = 0;
    virtual void visit(const StructWireBus& aObj) = 0;
    virtual void visit(const StructWireScalar& aObj) = 0;
    virtual void visit(const tagLOGFONTA& aObj) = 0;
};


class Component
{
public:
    virtual ~Component()
    { }

    virtual void accept(Visitor& aVisitor) const = 0;
};


#endif // VISITORPATTERN_HPP