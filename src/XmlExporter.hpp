#ifndef XMLEXPORTER_HPP
#define XMLEXPORTER_HPP


#include <filesystem>

// @todo Why does the compiler not find the lib? Check target_include_directory
#include "../build/vcpkg_installed/x64-linux/include/tinyxml2.h"

#include "Enums/Color.hpp"
#include "Library.hpp"
#include "Primitives/PrimArc.hpp"
#include "Primitives/PrimBezier.hpp"
#include "Primitives/PrimEllipse.hpp"
#include "Primitives/PrimLine.hpp"
#include "Primitives/PrimPolygon.hpp"
#include "Primitives/PrimPolyline.hpp"
#include "Primitives/PrimRect.hpp"
#include "Primitives/PrimSymbolVector.hpp"
#include "Streams/StreamPackage.hpp"
#include "Structures/StructSymbolDisplayProp.hpp"
#include "Structures/StructSymbolPin.hpp"
#include "Structures/StructSymbolPinBus.hpp"
#include "Structures/StructSymbolPinScalar.hpp"
#include "VisitorPattern.hpp"
#include "Win32/LOGFONTA.hpp"


namespace fs = std::filesystem;

using namespace tinyxml2;


class XmlExporter : public Visitor
{
public:

    XmlExporter(const fs::path& aExportPath) : mExportBasePath{aExportPath},
        mExportFilePath{}, mXml{}, mCurXmlElem{mXml.RootElement()}, mLib{nullptr}
    { }

    void exportXml()
    {
        const XMLError result = mXml.SaveFile(mExportFilePath.c_str());

        if(result != XML_SUCCESS)
        {
            throw std::runtime_error("Saving XML file to " + mExportFilePath.string()
                + " failed with error code " + std::to_string(result) + "!");
        }
    }

    virtual void visit(const Library& aObj) override;
    virtual void visit(const PageSettings& /* aObj */) override { }
    virtual void visit(const Point& /* aObj */) override { }
    virtual void visit(const PrimArc& aObj) override;
    virtual void visit(const PrimBezier& aObj) override;
    virtual void visit(const PrimBitmap& /* aObj */) override { }
    virtual void visit(const PrimCommentText& /* aObj */) override { }
    virtual void visit(const PrimEllipse& aObj) override;
    virtual void visit(const PrimLine& aObj) override;
    virtual void visit(const PrimPolygon& aObj) override;
    virtual void visit(const PrimPolyline& aObj) override;
    virtual void visit(const PrimRect& aObj) override;
    virtual void visit(const PrimSymbolVector& aObj) override;
    virtual void visit(const StreamAdminData& /* aObj */) override { }
    virtual void visit(const StreamCache& /* aObj */) override { }
    virtual void visit(const StreamDirectoryStruct& /* aObj */) override { }
    virtual void visit(const StreamDsnStream& /* aObj */) override { }
    virtual void visit(const StreamERC& /* aObj */) override { }
    virtual void visit(const StreamHierarchy& /* aObj */) override { }
    virtual void visit(const StreamHSObjects& /* aObj */) override { }
    virtual void visit(const StreamLibrary& /* aObj */) override { }
    virtual void visit(const StreamNetBundleMapData& /* aObj */) override { }
    virtual void visit(const StreamPackage& aObj) override;
    virtual void visit(const StreamPage& /* aObj */) override { }
    virtual void visit(const StreamSchematic& /* aObj */) override { }
    virtual void visit(const StreamSymbol& /* aObj */) override { }
    virtual void visit(const StreamType& /* aObj */) override { }
    virtual void visit(const StructAlias& /* aObj */) override { }
    virtual void visit(const StructBookMarkSymbolInst& /* aObj */) override { }
    virtual void visit(const StructBusEntry& /* aObj */) override { }
    virtual void visit(const StructERCSymbol& /* aObj */) override { }
    virtual void visit(const StructERCSymbolInst& /* aObj */) override { }
    virtual void visit(const StructGeneralProperties& /* aObj */) override { }
    virtual void visit(const StructGlobal& /* aObj */) override { }
    virtual void visit(const StructGlobalSymbol& /* aObj */) override { }
    virtual void visit(const StructGraphicArcInst& /* aObj */) override { }
    virtual void visit(const StructGraphicBezierInst& /* aObj */) override { }
    virtual void visit(const StructGraphicBitMapInst& /* aObj */) override { }
    virtual void visit(const StructGraphicBoxInst& /* aObj */) override { }
    virtual void visit(const StructGraphicCommentTextInst& /* aObj */) override { }
    virtual void visit(const StructGraphicEllipseInst& /* aObj */) override { }
    virtual void visit(const StructGraphicLineInst& /* aObj */) override { }
    virtual void visit(const StructGraphicOleEmbedInst& /* aObj */) override { }
    virtual void visit(const StructGraphicPolygonInst& /* aObj */) override { }
    virtual void visit(const StructGraphicPolylineInst& /* aObj */) override { }
    virtual void visit(const StructHierarchicSymbol& /* aObj */) override { }
    virtual void visit(const StructNetDbIdMapping& /* aObj */) override { }
    virtual void visit(const StructOffPageConnector& /* aObj */) override { }
    virtual void visit(const StructOffPageSymbol& /* aObj */) override { }
    virtual void visit(const StructPartInst& /* aObj */) override { }
    virtual void visit(const StructPinIdxMapping& /* aObj */) override { }
    virtual void visit(const StructPinShapeSymbol& /* aObj */) override { }
    virtual void visit(const StructPort& /* aObj */) override { }
    virtual void visit(const StructPrimitives& aObj) override;
    virtual void visit(const StructProperties& /* aObj */) override { }
    virtual void visit(const StructSthInHierarchy1& /* aObj */) override { }
    virtual void visit(const StructSthInHierarchy2& /* aObj */) override { }
    virtual void visit(const StructSthInHierarchy3& /* aObj */) override { }
    virtual void visit(const StructSthInPages0& /* aObj */) override { }
    virtual void visit(const StructSymbol& /* aObj */) override { }
    virtual void visit(const StructSymbolBBox& /* aObj */) override { }
    virtual void visit(const StructSymbolDisplayProp& aObj) override;
    virtual void visit(const StructSymbolPin& aObj) override;
    virtual void visit(const StructSymbolPinBus& aObj) override;
    virtual void visit(const StructSymbolPinScalar& aObj) override;
    virtual void visit(const StructT0x10& /* aObj */) override { }
    virtual void visit(const StructT0x1f& aObj) override;
    virtual void visit(const StructT0x34& /* aObj */) override { }
    virtual void visit(const StructT0x35& /* aObj */) override { }
    virtual void visit(const StructT0x45& /* aObj */) override { }
    virtual void visit(const StructT0x5b& /* aObj */) override { }
    virtual void visit(const StructTitleBlock& /* aObj */) override { }
    virtual void visit(const StructTitleBlockSymbol& /* aObj */) override { }
    virtual void visit(const StructWire& /* aObj */) override { }
    virtual void visit(const StructWireBus& /* aObj */) override { }
    virtual void visit(const StructWireScalar& /* aObj */) override { }
    virtual void visit(const tagLOGFONTA& aObj) override;

private:

    const fs::path mExportBasePath;
    fs::path mExportFilePath;

    XMLDocument mXml;
    XMLElement* mCurXmlElem;

    const Library* mLib;
};


#endif // XMLEXPORTER_HPP