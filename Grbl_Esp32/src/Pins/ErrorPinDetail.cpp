#include "ErrorPinDetail.h"
#include "../Assert.h"

namespace Pins {
    ErrorPinDetail::ErrorPinDetail(const PinOptionsParser& options) : PinDetail(0) {}

    PinCapabilities ErrorPinDetail::capabilities() const { return PinCapabilities::Error; }

    void ErrorPinDetail::write(int high) { Assert(false, "Cannot write to an error pin."); }
    int  ErrorPinDetail::read() { Assert(false, "Cannot read from an error pin."); }
    void ErrorPinDetail::setAttr(PinAttributes value) { /* Fine, this won't get you anywhere. */
    }
    PinAttributes ErrorPinDetail::getAttr() const { return PinAttributes::None; }

    String ErrorPinDetail::toString() const { return "ERROR_PIN"; }
}
