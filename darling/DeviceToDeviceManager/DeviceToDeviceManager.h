#define NO_D2D 1

// ok, so that define is supposedly what Xcode should generate when DeviceToDeviceManager isn't available
// but that's bogus: there's no code that uses it
// we have to define these to prevent compilation errors
//
// luckily, the actual implementation is weakly linked,
// so we don't even have to stub it

// no clue what it is, but this is usually a good guess for status types
typedef int D2DStatus;

typedef int D2DTransportType;

enum {
	D2DAWDLTransport,
	D2DWifiPeerToPeerTransport,
	D2DTransportMax,
};

// definitely a pointer, but not sure what it points to
typedef void* D2DServiceInstance;

// definitely an unsigned char (it's cast to an `mDNSu8`)
typedef unsigned char Byte;

enum {
	kD2DSuccess = 0, // 99.9999% of the time, this is what indicates success in status codes
};

typedef int D2DServiceEvent;

enum {
	D2DServiceFound,
	D2DServiceLost,
	D2DServiceResolved,
	D2DServiceRetained,
	D2DServiceReleased,
};

typedef void (*D2DServiceCallback)(D2DServiceEvent, D2DStatus, D2DServiceInstance, D2DTransportType, const Byte*, size_t, const Byte*, size_t, void*);
