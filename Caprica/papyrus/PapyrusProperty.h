#pragma once

#include <string>

#include <common/CapricaFileLocation.h>
#include <common/identifier_ref.h>
#include <common/IntrusiveLinkedList.h>
#include <common/LargelyBufferedString.h>

#include <papyrus/PapyrusFunction.h>
#include <papyrus/PapyrusResolutionContext.h>
#include <papyrus/PapyrusType.h>
#include <papyrus/PapyrusUserFlags.h>
#include <papyrus/PapyrusValue.h>

#include <pex/PexFile.h>
#include <pex/PexObject.h>

namespace caprica { namespace papyrus {

struct PapyrusProperty final
{
  identifier_ref name{ "" };
  identifier_ref autoVarName{ "" };
  identifier_ref documentationComment{ "" };
  PapyrusType type;
  PapyrusUserFlags userFlags{ };
  PapyrusFunction* readFunction{ nullptr };
  PapyrusFunction* writeFunction{ nullptr };
  PapyrusValue defaultValue{ PapyrusValue::Default() };

  CapricaFileLocation location;
  const PapyrusObject* parent{ nullptr };

  bool isAuto() const { return userFlags.isAuto; }
  bool isAutoReadOnly() const { return userFlags.isAutoReadOnly; }
  bool isConst() const { return userFlags.isConst; }

  bool isFunctionBacked() const { return isAutoReadOnly() || readFunction || writeFunction; }

  explicit PapyrusProperty(CapricaFileLocation loc, PapyrusType&& tp, const PapyrusObject* par) : location(loc), type(std::move(tp)), parent(par) { }
  PapyrusProperty(const PapyrusProperty&) = delete;
  ~PapyrusProperty() = default;

  void buildAutoVarName(allocators::ChainedPool* alloc) {
    LargelyBufferedString buf{ "::" };
    buf.append(name.to_string_view()).append("_var");
    this->autoVarName = alloc->allocateIdentifier(buf.data(), buf.size());
  }

  void buildPex(CapricaReportingContext& repCtx, pex::PexFile* file, pex::PexObject* obj) const;
  void semantic(PapyrusResolutionContext* ctx);
  void semantic2(PapyrusResolutionContext* ctx);

private:
  friend IntrusiveLinkedList<PapyrusProperty>;
  PapyrusProperty* next{ nullptr };
};

}}
