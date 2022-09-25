#include <iostream>
#define METAENUMERATOR_NAMESPACE MyRootNamespace
#include "MetaEnumerator.hpp"


namespace MyRootNamespace
{

// Forward declare enum types and a function which uses them (could be in a separate header)
enum class DocumentType;
using DocumentTypeMask = EnumeratorMask<DocumentType, 255>;

bool support_document_type(DocumentTypeMask documentTypes, DocumentType documentType);


enum class DocumentType
{
	NONE = 0,
	TEXT,
	IMAGE,
	WAVE,
	INHERITANCE,
	EXTENSION = 128, // reserve the first 127 values to built-in and inherited
	MAX = 255 // support at maximum 255 document types
};

template <>
class EnumeratorMeta<DocumentType> : public EnumeratorMetaDefault<DocumentType, false, 255>
{
public:
	static constexpr const bool logic_operators = true;
	static constexpr const bool string_operators = true;
	static constexpr const DocumentType MAX_VALUE = DocumentType::MAX;
	
	static constexpr const EnumEntry enum_entries[]{
		{ DocumentType::TEXT, "TEXT" },
		{ DocumentType::IMAGE, "IMAGE" },
		{ DocumentType::WAVE, "WAVE" }
	};
};

constexpr EnumeratorMeta<DocumentType>::EnumEntry EnumeratorMeta<DocumentType>::enum_entries[];


bool support_document_type(DocumentTypeMask documentTypes, DocumentType documentType)
{
	return documentTypes.has(documentType);
}


enum class DocumentType_Extended
{
	NONE = 0,
	RICHTEXT = EnumeratorMeta<DocumentType>::Inheritor::inherit(),
	SPREADSHEET,
	VIDEO,
	MAX = EnumeratorMeta<DocumentType>::Inheritor::inheritMaximum()
};

template <>
class EnumeratorMeta<DocumentType_Extended> : public EnumeratorMetaDefault<DocumentType_Extended, false, 255>
{
public:
	using BaseEnumType = DocumentType;
	using EnumEntry = EnumeratorMeta<DocumentType>::EnumEntry;
	
	static constexpr const bool logic_operators = true;
	static constexpr const bool string_operators = true;
	static constexpr const DocumentType_Extended MAX_VALUE = DocumentType_Extended::MAX;
	
	static constexpr const EnumEntry enum_entries[]{
		{ +DocumentType_Extended::RICHTEXT, "RICHTEXT" },
		{ +DocumentType_Extended::SPREADSHEET, "SPREADSHEET" },
		{ +DocumentType_Extended::VIDEO, "VIDEO" }
	};
};

template <>
class EnumeratorInherited<DocumentType>
{
public:
	using InheritedType = DocumentType_Extended;
};

constexpr EnumeratorMeta<DocumentType_Extended>::EnumEntry EnumeratorMeta<DocumentType_Extended>::enum_entries[];

}


int main(int argc, char *argv[])
{
	using namespace MyRootNamespace;
	
	DocumentTypeMask textTypes = DocumentType::TEXT | +DocumentType_Extended::RICHTEXT;
	
	std::cout << "Support for " << DocumentType_Extended::VIDEO << " document enabled: " << support_document_type(textTypes, +DocumentType_Extended::VIDEO) << "\n";
	// prints: Support for VIDEO document enabled: 0
	
	std::cout << "Support for " << DocumentType_Extended::RICHTEXT << " document enabled: " << support_document_type(textTypes, +DocumentType_Extended::RICHTEXT) << "\n";
	// prints: Support for VIDEO document enabled: 1
	
	// Extending the enum at run-time, to record dynamic document types (for instance, added by plugins or external modules)
	using Extender = EnumeratorMeta<DocumentType>::Extender;
	auto documentType1 = Extender::extend("documentType1");
	auto documentType2 = Extender::extend("documentType2");
	DocumentTypeMask pluginTypes = documentType1 | documentType2;
	std::cout << "Is " << DocumentType_Extended::RICHTEXT << " registered by plugin: " << pluginTypes.has(+DocumentType_Extended::RICHTEXT) << "\n";
	// prints: Is RICHTEXT registered by plugin: 0
	std::cout << "Is " << documentType1 << " registered by plugin: " << pluginTypes.has(documentType1) << "\n";
	// prints: Is documentType1 registered by plugin: 1
	
	using Serializer = EnumeratorSerializer<DocumentType>;
	std::cout << "Unserialized values " << (int)Serializer::get_value("documentType1") << ":" << Serializer::get_value("documentType1") << ", " << (int)Serializer::get_value("RICHTEXT") << ":" <<Serializer::get_value("RICHTEXT") << ", " << (int)Serializer::get_value("WAVE") << ":" <<Serializer::get_value("WAVE");
	// prints: Unserialized values 128:documentType1, 4:RICHTEXT, 3:WAVE
	
	std::cout << "\n";
}