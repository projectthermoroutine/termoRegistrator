// dllmain.h: объ€вление класса модул€.

class CThermoRoutineModule : public ATL::CAtlDllModuleT< CThermoRoutineModule >
{
public :
	DECLARE_LIBID(LIBID_ThermoRoutineLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_THERMOROUTINE, "{43607D0F-ED49-4BC9-B91D-1FE5EF701320}")
};

extern class CThermoRoutineModule _AtlModule;
