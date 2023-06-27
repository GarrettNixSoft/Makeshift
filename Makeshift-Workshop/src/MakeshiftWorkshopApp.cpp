#include <Makeshift.hpp>

// ---------------- Entry Point ---------------------
#include <Makeshift/Core/EntryPoint.hpp>
// --------------------------------------------------

#include "WorkshopLayer.hpp"

namespace Makeshift {

	class MakeshiftWorkshop : public Application {

	public:
		MakeshiftWorkshop() : Application("Makeshift Workshop") {
			pushLayer(new WorkshopLayer());
		}

		~MakeshiftWorkshop() {

		}

	};


	Application* Makeshift::CreateApplication() {
		return new MakeshiftWorkshop();
	}

}