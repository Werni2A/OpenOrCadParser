#include "XmlExporter.hpp"

void OOCP::XmlExporter::visit(const Library& aObj)
{
    mLib = &aObj;

    for(const auto& package : aObj.packages)
    {
        mXml.Clear();

        XMLDeclaration* std_declaration = mXml.NewDeclaration();

        mXml.InsertFirstChild(std_declaration);

        XMLElement* eLib = mXml.NewElement("Lib");
        mXml.InsertEndChild(eLib);

        mCurXmlElem = eLib;

        if(package)
        {
            package->accept(*this);

            // @todo Use Package name
            static uint32_t PackageCtr = 0;
            const std::string pkgName  = "PackageName" + std::to_string(PackageCtr++) + ".xml";
            mExportFilePath            = mExportBasePath / pkgName;

            exportXml();
        }
    }
}

void OOCP::XmlExporter::visit(const PrimArc& aObj)
{
    XMLElement* parent = mCurXmlElem;

    XMLElement* eArc = mXml.NewElement("Arc");
    mCurXmlElem->InsertEndChild(eArc);

    mCurXmlElem = eArc;

    XMLElement* eDefn = mXml.NewElement("Defn");
    mCurXmlElem->InsertEndChild(eDefn);

    eDefn->SetAttribute("endX", aObj.endX);
    eDefn->SetAttribute("endY", aObj.endY);
    eDefn->SetAttribute("lineStyle", static_cast<uint32_t>(aObj.getLineStyle()));
    eDefn->SetAttribute("lineWidth", static_cast<uint32_t>(aObj.getLineWidth()));
    eDefn->SetAttribute("startX", aObj.startX);
    eDefn->SetAttribute("startY", aObj.startY);
    eDefn->SetAttribute("x1", aObj.x1);
    eDefn->SetAttribute("x2", aObj.x2);
    eDefn->SetAttribute("y1", aObj.y1);
    eDefn->SetAttribute("y2", aObj.y2);

    mCurXmlElem = parent;
}

void OOCP::XmlExporter::visit(const PrimBezier& aObj)
{
    XMLElement* parent = mCurXmlElem;

    XMLElement* eBezier = mXml.NewElement("Bezier");
    mCurXmlElem->InsertEndChild(eBezier);

    mCurXmlElem = eBezier;

    XMLElement* eDefn = mXml.NewElement("Defn");
    mCurXmlElem->InsertEndChild(eDefn);

    eDefn->SetAttribute("lineStyle", static_cast<uint32_t>(aObj.getLineStyle()));
    eDefn->SetAttribute("lineWidth", static_cast<uint32_t>(aObj.getLineWidth()));

    for(const auto& point : aObj.points)
    {
        XMLElement* eBezierPoint = mXml.NewElement("BezierPoint");
        mCurXmlElem->InsertEndChild(eBezierPoint);

        XMLElement* eDefn = mXml.NewElement("Defn");
        eBezierPoint->InsertEndChild(eDefn);

        eDefn->SetAttribute("x", point.x);
        eDefn->SetAttribute("y", point.y);
    }

    mCurXmlElem = parent;
}

void OOCP::XmlExporter::visit(const PrimEllipse& aObj)
{
    XMLElement* parent = mCurXmlElem;

    XMLElement* eEllipse = mXml.NewElement("Ellipse");
    mCurXmlElem->InsertEndChild(eEllipse);

    mCurXmlElem = eEllipse;

    XMLElement* eDefn = mXml.NewElement("Defn");
    mCurXmlElem->InsertEndChild(eDefn);

    eDefn->SetAttribute("fillStyle", static_cast<uint32_t>(aObj.getFillStyle()));
    eDefn->SetAttribute("hatchStyle", static_cast<uint32_t>(aObj.getHatchStyle()));
    eDefn->SetAttribute("lineStyle", static_cast<uint32_t>(aObj.getLineStyle()));
    eDefn->SetAttribute("lineWidth", static_cast<uint32_t>(aObj.getLineWidth()));
    eDefn->SetAttribute("x1", aObj.x1);
    eDefn->SetAttribute("x2", aObj.x2);
    eDefn->SetAttribute("y1", aObj.y1);
    eDefn->SetAttribute("y2", aObj.y2);

    mCurXmlElem = parent;
}

void OOCP::XmlExporter::visit(const PrimRect& aObj)
{
    XMLElement* parent = mCurXmlElem;

    XMLElement* eRect = mXml.NewElement("Rect");
    mCurXmlElem->InsertEndChild(eRect);

    mCurXmlElem = eRect;

    XMLElement* eDefn = mXml.NewElement("Defn");
    mCurXmlElem->InsertEndChild(eDefn);

    eDefn->SetAttribute("fillStyle", static_cast<uint32_t>(aObj.fillStyle));
    eDefn->SetAttribute("hatchStyle", static_cast<uint32_t>(aObj.hatchStyle));
    eDefn->SetAttribute("lineStyle", static_cast<uint32_t>(aObj.getLineStyle()));
    eDefn->SetAttribute("lineWidth", static_cast<uint32_t>(aObj.getLineWidth()));
    eDefn->SetAttribute("x1", aObj.x1);
    eDefn->SetAttribute("x2", aObj.x2);
    eDefn->SetAttribute("y1", aObj.y1);
    eDefn->SetAttribute("y2", aObj.y2);

    mCurXmlElem = parent;
}

void OOCP::XmlExporter::visit(const PrimPolygon& aObj)
{
    XMLElement* parent = mCurXmlElem;

    XMLElement* ePolygon = mXml.NewElement("Polygon");
    mCurXmlElem->InsertEndChild(ePolygon);

    mCurXmlElem = ePolygon;

    XMLElement* eDefn = mXml.NewElement("Defn");
    mCurXmlElem->InsertEndChild(eDefn);

    eDefn->SetAttribute("fillStyle", static_cast<uint32_t>(aObj.fillStyle));
    eDefn->SetAttribute("hatchStyle", static_cast<uint32_t>(aObj.hatchStyle));
    eDefn->SetAttribute("lineStyle", static_cast<uint32_t>(aObj.getLineStyle()));
    eDefn->SetAttribute("lineWidth", static_cast<uint32_t>(aObj.getLineWidth()));

    for(const auto& point : aObj.points)
    {
        XMLElement* ePolygonPoint = mXml.NewElement("PolygonPoint");
        mCurXmlElem->InsertEndChild(ePolygonPoint);

        XMLElement* eDefn = mXml.NewElement("Defn");
        ePolygonPoint->InsertEndChild(eDefn);

        eDefn->SetAttribute("x", point.x);
        eDefn->SetAttribute("y", point.y);
    }

    mCurXmlElem = parent;
}

void OOCP::XmlExporter::visit(const PrimPolyline& aObj)
{
    XMLElement* parent = mCurXmlElem;

    XMLElement* ePolyline = mXml.NewElement("Polyline");
    mCurXmlElem->InsertEndChild(ePolyline);

    mCurXmlElem = ePolyline;

    XMLElement* eDefn = mXml.NewElement("Defn");
    mCurXmlElem->InsertEndChild(eDefn);

    eDefn->SetAttribute("lineStyle", static_cast<uint32_t>(aObj.getLineStyle()));
    eDefn->SetAttribute("lineWidth", static_cast<uint32_t>(aObj.getLineWidth()));

    for(const auto& point : aObj.points)
    {
        XMLElement* ePolylinePoint = mXml.NewElement("PolylinePoint");
        mCurXmlElem->InsertEndChild(ePolylinePoint);

        XMLElement* eDefn = mXml.NewElement("Defn");
        ePolylinePoint->InsertEndChild(eDefn);

        eDefn->SetAttribute("x", point.x);
        eDefn->SetAttribute("y", point.y);
    }

    mCurXmlElem = parent;
}

void OOCP::XmlExporter::visit(const PrimLine& aObj)
{
    XMLElement* parent = mCurXmlElem;

    XMLElement* eLine = mXml.NewElement("Line");
    mCurXmlElem->InsertEndChild(eLine);

    mCurXmlElem = eLine;

    XMLElement* eDefn = mXml.NewElement("Defn");
    mCurXmlElem->InsertEndChild(eDefn);

    eDefn->SetAttribute("lineStyle", static_cast<uint32_t>(aObj.getLineStyle()));
    eDefn->SetAttribute("lineWidth", static_cast<uint32_t>(aObj.getLineWidth()));
    eDefn->SetAttribute("x1", aObj.x1);
    eDefn->SetAttribute("x2", aObj.x2);
    eDefn->SetAttribute("y1", aObj.y1);
    eDefn->SetAttribute("y2", aObj.y2);

    mCurXmlElem = parent;
}

void OOCP::XmlExporter::visit(const PrimSymbolVector& aObj)
{
    XMLElement* parent = mCurXmlElem;

    XMLElement* eSymbolVector = mXml.NewElement("SymbolVector");
    mCurXmlElem->InsertEndChild(eSymbolVector);

    mCurXmlElem = eSymbolVector;

    {
        XMLElement* eDefn = mXml.NewElement("Defn");
        mCurXmlElem->InsertEndChild(eDefn);

        eDefn->SetAttribute("locX", aObj.locX);
        eDefn->SetAttribute("locY", aObj.locY);
        eDefn->SetAttribute("name", aObj.name.c_str());
    }

    for(const auto& primitive : aObj.primitives)
    {
        primitive->accept(*this);
    }

    mCurXmlElem = parent;
}

void OOCP::XmlExporter::visit(const StructSymbolDisplayProp& aObj)
{
    XMLElement* parent = mCurXmlElem;

    XMLElement* eSymbolDisplayProp = mXml.NewElement("SymbolDisplayProp");
    mCurXmlElem->InsertEndChild(eSymbolDisplayProp);

    mCurXmlElem = eSymbolDisplayProp;

    XMLElement* eDefn = mXml.NewElement("Defn");
    mCurXmlElem->InsertEndChild(eDefn);

    eDefn->SetAttribute("locX", aObj.x);
    eDefn->SetAttribute("locY", aObj.y);
    eDefn->SetAttribute("name", aObj.nameIdx); // @todo Value itself not index
    eDefn->SetAttribute("rotation", static_cast<uint32_t>(aObj.rotation));
    eDefn->SetAttribute("textJustification", "0"); // @todo

    XMLElement* ePropFont = mXml.NewElement("PropFont");
    mCurXmlElem->InsertEndChild(ePropFont);

    if(mLib && mLib->library && aObj.textFontIdx < mLib->library->textFonts.size())
    {
        XMLElement* parent = mCurXmlElem;

        mCurXmlElem = ePropFont;

        mLib->library->textFonts[aObj.textFontIdx].accept(*this);

        mCurXmlElem = parent;
    }

    XMLElement* ePropColor = mXml.NewElement("PropColor");
    mCurXmlElem->InsertEndChild(ePropColor);

    XMLElement* eDefn2 = mXml.NewElement("Defn");
    ePropColor->InsertEndChild(eDefn2);

    eDefn2->SetAttribute("val", static_cast<uint32_t>(aObj.propColor));

    XMLElement* ePropDispType = mXml.NewElement("PropDispType");
    mCurXmlElem->InsertEndChild(ePropDispType);

    XMLElement* eDefn3 = mXml.NewElement("Defn");
    ePropDispType->InsertEndChild(eDefn3);

    eDefn3->SetAttribute("ValueIfValueExist", 1); // @todo
    eDefn3->SetAttribute("val", 1);               // @todo

    mCurXmlElem = parent;
}

void OOCP::XmlExporter::visit(const StructSymbolPin& aObj)
{
    XMLElement* eDefn = mXml.NewElement("Defn");
    mCurXmlElem->InsertEndChild(eDefn);

    eDefn->SetAttribute("hotptX", aObj.hotptX);
    eDefn->SetAttribute("hotptY", aObj.hotptY);
    eDefn->SetAttribute("name", aObj.name.c_str());
    eDefn->SetAttribute("position", 0); // @todo
    eDefn->SetAttribute("startX", aObj.startX);
    eDefn->SetAttribute("startY", aObj.startY);
    eDefn->SetAttribute("type", static_cast<uint32_t>(aObj.portType));
    eDefn->SetAttribute("visible", 1); // @todo

    {
        XMLElement* eIsLong = mXml.NewElement("IsLong");
        mCurXmlElem->InsertEndChild(eIsLong);

        XMLElement* eDefn = mXml.NewElement("Defn");
        eIsLong->InsertEndChild(eDefn);

        eDefn->SetAttribute("val", static_cast<uint32_t>(aObj.pinShape.isLong));
    }

    {
        XMLElement* eIsClock = mXml.NewElement("IsClock");
        mCurXmlElem->InsertEndChild(eIsClock);

        XMLElement* eDefn = mXml.NewElement("Defn");
        eIsClock->InsertEndChild(eDefn);

        eDefn->SetAttribute("val", static_cast<uint32_t>(aObj.pinShape.isClock));
    }

    {
        XMLElement* eIsDot = mXml.NewElement("IsDot");
        mCurXmlElem->InsertEndChild(eIsDot);

        XMLElement* eDefn = mXml.NewElement("Defn");
        eIsDot->InsertEndChild(eDefn);

        eDefn->SetAttribute("val", static_cast<uint32_t>(aObj.pinShape.isDot));
    }

    {
        XMLElement* eIsLeftPointing = mXml.NewElement("IsLeftPointing");
        mCurXmlElem->InsertEndChild(eIsLeftPointing);

        XMLElement* eDefn = mXml.NewElement("Defn");
        eIsLeftPointing->InsertEndChild(eDefn);

        eDefn->SetAttribute("val", static_cast<uint32_t>(aObj.pinShape.isLeftPointing));
    }

    {
        XMLElement* eIsRightPointing = mXml.NewElement("IsRightPointing");
        mCurXmlElem->InsertEndChild(eIsRightPointing);

        XMLElement* eDefn = mXml.NewElement("Defn");
        eIsRightPointing->InsertEndChild(eDefn);

        eDefn->SetAttribute("val", static_cast<uint32_t>(aObj.pinShape.isRightPointing));
    }

    {
        XMLElement* eIsNetStyle = mXml.NewElement("IsNetStyle");
        mCurXmlElem->InsertEndChild(eIsNetStyle);

        XMLElement* eDefn = mXml.NewElement("Defn");
        eIsNetStyle->InsertEndChild(eDefn);

        eDefn->SetAttribute("val", static_cast<uint32_t>(aObj.pinShape.isNetStyle));
    }

    {
        XMLElement* eIsNoConnect = mXml.NewElement("IsNoConnect");
        mCurXmlElem->InsertEndChild(eIsNoConnect);

        XMLElement* eDefn = mXml.NewElement("Defn");
        eIsNoConnect->InsertEndChild(eDefn);

        eDefn->SetAttribute("val", static_cast<uint32_t>(aObj.pinShape.isNoConnect));
    }

    {
        XMLElement* eIsGlobal = mXml.NewElement("IsGlobal");
        mCurXmlElem->InsertEndChild(eIsGlobal);

        XMLElement* eDefn = mXml.NewElement("Defn");
        eIsGlobal->InsertEndChild(eDefn);

        eDefn->SetAttribute("val", static_cast<uint32_t>(aObj.pinShape.isGlobal));
    }

    {
        XMLElement* eIsNumberVisible = mXml.NewElement("IsNumberVisible");
        mCurXmlElem->InsertEndChild(eIsNumberVisible);

        XMLElement* eDefn = mXml.NewElement("Defn");
        eIsNumberVisible->InsertEndChild(eDefn);

        eDefn->SetAttribute("val", 1 /* static_cast<uint32_t>(aObj.pinShape.isNumberVisible) */); // @todo
    }
}

void OOCP::XmlExporter::visit(const StructSymbolPinBus& aObj)
{
    XMLElement* parent = mCurXmlElem;

    XMLElement* eSymbolPinBus = mXml.NewElement("SymbolPinBus");
    mCurXmlElem->InsertEndChild(eSymbolPinBus);

    mCurXmlElem = eSymbolPinBus;

    visit(static_cast<const StructSymbolPin&>(aObj));

    mCurXmlElem = parent;
}

void OOCP::XmlExporter::visit(const StructSymbolPinScalar& aObj)
{
    XMLElement* parent = mCurXmlElem;

    XMLElement* eSymbolPinScalar = mXml.NewElement("SymbolPinScalar");
    mCurXmlElem->InsertEndChild(eSymbolPinScalar);

    mCurXmlElem = eSymbolPinScalar;

    visit(static_cast<const StructSymbolPin&>(aObj));

    mCurXmlElem = parent;
}

void OOCP::XmlExporter::visit(const StructPackage& aObj)
{
    XMLElement* parent = mCurXmlElem;

    XMLElement* ePhysicalPart = mXml.NewElement("PhysicalPart");
    mCurXmlElem->InsertEndChild(ePhysicalPart);

    mCurXmlElem = ePhysicalPart;

    for(size_t i = 0U; i < aObj.devices.size(); ++i)
    {
        assert(aObj.devices.at(i)->pinMap.size() == aObj.devices.at(i)->pinIgnore.size());
        assert(aObj.devices.at(i)->pinMap.size() == aObj.devices.at(i)->pinGroup.size());

        for(size_t position = 0U; position < aObj.devices.at(i)->pinMap.size(); ++position)
        {
            const auto& number = aObj.devices.at(i)->pinMap.at(position);
            const auto& shared = aObj.devices.at(i)->pinIgnore.at(position);
            const auto& swapid = aObj.devices.at(i)->pinGroup.at(position);

            {
                XMLElement* parent = mCurXmlElem;

                XMLElement* ePinNumber = mXml.NewElement("PinNumber");
                mCurXmlElem->InsertEndChild(ePinNumber);

                mCurXmlElem = ePinNumber;

                XMLElement* eDefn = mXml.NewElement("Defn");
                mCurXmlElem->InsertEndChild(eDefn);

                eDefn->SetAttribute("number", number.c_str());
                eDefn->SetAttribute("position", position);

                mCurXmlElem = parent;
            }

            {
                XMLElement* parent = mCurXmlElem;

                XMLElement* ePinShared = mXml.NewElement("PinShared");
                mCurXmlElem->InsertEndChild(ePinShared);

                mCurXmlElem = ePinShared;

                XMLElement* eDefn = mXml.NewElement("Defn");
                mCurXmlElem->InsertEndChild(eDefn);

                eDefn->SetAttribute("position", position);
                eDefn->SetAttribute("shared", static_cast<uint32_t>(shared));

                mCurXmlElem = parent;
            }

            {
                XMLElement* parent = mCurXmlElem;

                XMLElement* ePinSwap = mXml.NewElement("PinSwap");
                mCurXmlElem->InsertEndChild(ePinSwap);

                mCurXmlElem = ePinSwap;

                XMLElement* eDefn = mXml.NewElement("Defn");
                mCurXmlElem->InsertEndChild(eDefn);

                eDefn->SetAttribute("position", position);
                eDefn->SetAttribute("swapid", swapid);

                mCurXmlElem = parent;
            }
        }
    }

    mCurXmlElem = parent;
}

void OOCP::XmlExporter::visit(const StreamPackage& aObj)
{
    XMLElement* parent = mCurXmlElem;

    const std::string normal = ".Normal";
    std::string normalName   = aObj.properties.at(0)->normalName;
    normalName.resize(normalName.size() - normal.size());

    XMLElement* ePackage = mXml.NewElement("Package");
    mCurXmlElem->InsertEndChild(ePackage);

    mCurXmlElem = ePackage;

    XMLElement* eDefn = mXml.NewElement("Defn");
    mCurXmlElem->InsertEndChild(eDefn);

    eDefn->SetAttribute("alphabeticNumbering", "0"); // @todo
    eDefn->SetAttribute("isHomogeneous", "1");       // @todo
    eDefn->SetAttribute("name", normalName.c_str());
    eDefn->SetAttribute("refdesPrefix", "unimplemented"); // @todo
    eDefn->SetAttribute("pcbLib", "unimplemented");       // @todo
    eDefn->SetAttribute("pcbFootprint", "unimplemented"); // @todo
    eDefn->SetAttribute("sourcelib", "unimplemented");    // @todo
    eDefn->SetAttribute("timestamp", "0");                // @todo
    eDefn->SetAttribute("timezone", "0");                 // @todo

    {
        XMLElement* parent = mCurXmlElem;

        XMLElement* eLibPart = mXml.NewElement("LibPart");
        mCurXmlElem->InsertEndChild(eLibPart);

        mCurXmlElem = eLibPart;

        {
            XMLElement* eDefn = mXml.NewElement("Defn");
            mCurXmlElem->InsertEndChild(eDefn);

            eDefn->SetAttribute("CellName", normalName.c_str());
        }

        for(const auto& primitive : aObj.primitives)
        {
            if(primitive)
            {
                primitive->accept(*this);
            }
        }

        if(aObj.package)
        {
            visit(*aObj.package);
        }

        mCurXmlElem = parent;
    }

    mCurXmlElem = parent;
}

void OOCP::XmlExporter::visit(const StructLibraryPart& aObj)
{
    XMLElement* parent = mCurXmlElem;

    XMLElement* eNormalView = mXml.NewElement("NormalView");
    mCurXmlElem->InsertEndChild(eNormalView);

    mCurXmlElem = eNormalView;

    {
        XMLElement* eSymbolBBox = mXml.NewElement("SymbolBBox");
        mCurXmlElem->InsertEndChild(eSymbolBBox);

        XMLElement* eDefn = mXml.NewElement("Defn");
        eSymbolBBox->InsertEndChild(eDefn);

        eDefn->SetAttribute("x1", "0");  // @todo
        eDefn->SetAttribute("x2", "30"); // @todo
        eDefn->SetAttribute("y1", "0");  // @todo
        eDefn->SetAttribute("y2", "60"); // @todo
    }

    {
        XMLElement* eIsPinNumbersVisible = mXml.NewElement("IsPinNumbersVisible");
        mCurXmlElem->InsertEndChild(eIsPinNumbersVisible);

        XMLElement* eDefn = mXml.NewElement("Defn");
        eIsPinNumbersVisible->InsertEndChild(eDefn);

        eDefn->SetAttribute("val", static_cast<uint32_t>(aObj.generalProperties.pinNumberVisible));
    }

    {
        XMLElement* eIsPinNamesRotated = mXml.NewElement("IsPinNamesRotated");
        mCurXmlElem->InsertEndChild(eIsPinNamesRotated);

        XMLElement* eDefn = mXml.NewElement("Defn");
        eIsPinNamesRotated->InsertEndChild(eDefn);

        eDefn->SetAttribute("val", static_cast<uint32_t>(aObj.generalProperties.pinNameRotate));
    }

    {
        XMLElement* eIsPinNamesVisible = mXml.NewElement("IsPinNamesVisible");
        mCurXmlElem->InsertEndChild(eIsPinNamesVisible);

        XMLElement* eDefn = mXml.NewElement("Defn");
        eIsPinNamesVisible->InsertEndChild(eDefn);

        eDefn->SetAttribute("val", static_cast<uint32_t>(aObj.generalProperties.pinNameVisible));
    }

    {
        XMLElement* ePartValue = mXml.NewElement("PartValue");
        mCurXmlElem->InsertEndChild(ePartValue);

        XMLElement* eDefn = mXml.NewElement("Defn");
        ePartValue->InsertEndChild(eDefn);

        eDefn->SetAttribute("name", aObj.generalProperties.partValue.c_str());
    }

    for(const auto& primitive : aObj.primitives)
    {
        if(primitive)
        {
            primitive->accept(*this);
        }
    }

    for(const auto& symbolPin : aObj.symbolPins)
    {
        if(symbolPin)
        {
            symbolPin->accept(*this);
        }
    }

    for(const auto& symbolDisplayProp : aObj.symbolDisplayProps)
    {
        if(symbolDisplayProp)
        {
            symbolDisplayProp->accept(*this);
        }
    }

    mCurXmlElem = parent;
}

void OOCP::XmlExporter::visit(const tagLOGFONTA& aObj)
{
    XMLElement* parent = mCurXmlElem;

    XMLElement* eDefn = mXml.NewElement("Defn");
    mCurXmlElem->InsertEndChild(eDefn);

    eDefn->SetAttribute("escapement", aObj.lfEscapement);
    eDefn->SetAttribute("height", aObj.lfHeight);
    eDefn->SetAttribute("italic", aObj.lfItalic);
    eDefn->SetAttribute("name", aObj.lfFaceName);
    eDefn->SetAttribute("orientation", aObj.lfOrientation);
    eDefn->SetAttribute("weight", aObj.lfWeight);
    eDefn->SetAttribute("width", aObj.lfWidth);
    eDefn->SetAttribute("charset", aObj.lfCharSet);

    mCurXmlElem = parent;
}