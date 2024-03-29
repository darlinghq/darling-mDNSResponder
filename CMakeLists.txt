project(darling-mDNSResponder)

include_directories(
	darling
	mDNSShared
	mDNSCore
	mDNSMacOSX
	mDNSPosix
	DSO

	${CMAKE_SOURCE_DIR}/${DARLING_SDK_RELATIVE_PATH}/usr/include/libarchive
)

add_compile_definitions(
	__APPLE_USE_RFC_3542=1
	APPLE_OSX_mDNSResponder=1
	__MigTypeCheck=1
	"mDNSResponderVersion=\"(Engineering Build)\""
	LEGACY_NAT_TRAVERSAL_
	_BUILDING_XCODE_PROJECT_=1
	USE_LIBIDN=1
	USE_SYSTEMCONFIGURATION_PRIVATE_HEADERS

	PRIVATE=1
)

add_compile_options(
	-fwrapv
)

#
# libSystem component (`libsystem_dnssd`)
#
set(DYLIB_INSTALL_NAME "/usr/lib/system/libsystem_dnssd.dylib")
set(DYLIB_COMPAT_VERSION "1.0.0")
set(DYLIB_CURRENT_VERSION "1096.60.2")

add_circular(system_dnssd
	FAT
	SOURCES
		mDNSMacOSX/DNSServiceDiscovery.c
		mDNSShared/dnssd_clientlib.c
		mDNSShared/dnssd_clientstub.c
		mDNSShared/dnssd_ipc.c
	SIBLINGS
		system_dyld
		compiler_rt
		system_kernel
		platform
		system_pthread
		system_malloc
		system_c
		system_blocks
		libdispatch_shared
		system_asl
)

target_compile_definitions(system_dnssd PRIVATE
	__DARWIN_NON_CANCELABLE=1
)

install(TARGETS system_dnssd DESTINATION libexec/darling/usr/lib/system)

#
# mDNSResponder binary
#
add_darling_executable(mDNSResponder
	Clients/dnssdutil/DNSMessage.c
	DSO/dso-transport.c
	DSO/dso.c
	mDNSCore/DNSCommon.c
	mDNSCore/DNSDigest.c
	mDNSCore/dnsproxy.c
	mDNSCore/mDNS.c
	mDNSCore/uDNS.c
	mDNSMacOSX/BLE.c
	mDNSMacOSX/coreBLE.m
	mDNSMacOSX/D2D.c
	mDNSMacOSX/daemon.c
	mDNSMacOSX/DNS64.c
	mDNSMacOSX/DNSHeuristics.m
	mDNSMacOSX/DNSProxySupport.c
	mDNSMacOSX/dnssd_analytics.c
	mDNSMacOSX/dnssd_server.c
	mDNSMacOSX/dnssd_svcb.c
	mDNSMacOSX/dnssd_xpc.c
	mDNSMacOSX/dnssec_v2/dnssec_v2_client.c
	mDNSMacOSX/dnssec_v2/dnssec_v2_crypto.c
	mDNSMacOSX/dnssec_v2/dnssec_v2_helper.c
	mDNSMacOSX/dnssec_v2/dnssec_v2_retrieval.c
	mDNSMacOSX/dnssec_v2/dnssec_v2_structs.c
	mDNSMacOSX/dnssec_v2/dnssec_v2_trust_anchor.c
	mDNSMacOSX/dnssec_v2/dnssec_v2_validation.c
	mDNSMacOSX/dnssec_v2/dnssec_v2.c
	mDNSMacOSX/dnssec_v2/utilities/base_encoding/base_n.c
	mDNSMacOSX/dnssec_v2/utilities/list/list.c
	mDNSMacOSX/helper-stubs.c
	mDNSMacOSX/HTTPUtilities.m
	mDNSMacOSX/LegacyNATTraversal.c
	mDNSMacOSX/mdns_objects/mdns_address.c
	mDNSMacOSX/mdns_objects/mdns_dns_service.c
	mDNSMacOSX/mdns_objects/mdns_helpers.c
	mDNSMacOSX/mdns_objects/mdns_interface_monitor.c
	mDNSMacOSX/mdns_objects/mdns_managed_defaults.c
	mDNSMacOSX/mdns_objects/mdns_message.c
	mDNSMacOSX/mdns_objects/mdns_object.c
	mDNSMacOSX/mdns_objects/mdns_objects.m
	mDNSMacOSX/mdns_objects/mdns_powerlog.c
	mDNSMacOSX/mdns_objects/mdns_resolver.c
	mDNSMacOSX/mdns_objects/mdns_set.c
	mDNSMacOSX/mdns_objects/mdns_symptoms.c
	mDNSMacOSX/mdns_objects/mdns_tlv.c
	mDNSMacOSX/mdns_objects/mdns_trust_checks.m
	mDNSMacOSX/mdns_objects/mdns_trust.c
	mDNSMacOSX/mdns_objects/mdns_xpc.c
	mDNSMacOSX/mDNSMacOSX.c
	mDNSMacOSX/Metrics.m
	mDNSMacOSX/Private/advertising_proxy_services.c
	mDNSMacOSX/QuerierSupport.c
	mDNSMacOSX/SymptomReporter.c
	mDNSMacOSX/uDNSPathEvaluation.c
	mDNSMacOSX/utilities/system_utilities.m
	mDNSMacOSX/xpc_services/xpc_service_dns_proxy.c
	mDNSMacOSX/xpc_services/xpc_service_log_utility.c
	mDNSMacOSX/xpc_services/xpc_services.c
	mDNSPosix/posix_utilities.c
	mDNSShared/ClientRequests.c
	mDNSShared/dnssd_clientshim.c
	mDNSShared/dnssd_ipc.c
	mDNSShared/mDNSDebug.c
	mDNSShared/PlatformCommon.c
	mDNSShared/uds_daemon.c
)

target_link_libraries(mDNSResponder
	archive
	CoreFoundation
	Foundation
	IOKit
	Security
	SystemConfiguration
	icucore
	xml2
	libnetwork

	system
)

# the Xcode project says ObjC ARC is enabled for this target, but the actual code doesn't build with ARC enabled...
#target_compile_options(mDNSResponder PRIVATE
#	-fobjc-arc
#)

target_compile_definitions(mDNSResponder PRIVATE
	MDNSRESPONDER_PLATFORM_APPLE=1
	DSO_USES_NETWORK_FRAMEWORK
)

target_include_directories(mDNSResponder PRIVATE
	mDNSMacOSX/utilities
	mDNSMacOSX/xpc_services
)

install(FILES mDNSShared/mDNSResponder.8 DESTINATION libexec/darling/usr/share/man/man8)
install(FILES mDNSMacOSX/com.apple.mDNSResponder.plist DESTINATION libexec/darling/System/Library/LaunchDaemons)
install(TARGETS mDNSResponder DESTINATION libexec/darling/usr/sbin)

#
# mDNSResponderHelper binary
#
add_darling_executable(mDNSResponderHelper
	mDNSMacOSX/helper-main.c
	mDNSMacOSX/helper.c
	mDNSMacOSX/P2PPacketFilter.c
	mDNSMacOSX/utilities/setup_assistant_helper.m
)

target_link_libraries(mDNSResponderHelper
	IOKit
	bsm.0
	CoreFoundation
	SystemConfiguration
	Security

	system
)

install(FILES mDNSMacOSX/mDNSResponderHelper.8 DESTINATION libexec/darling/usr/share/man/man8)
install(FILES mDNSMacOSX/com.apple.mDNSResponderHelper.plist DESTINATION libexec/darling/System/Library/LaunchDaemons)
install(TARGETS mDNSResponderHelper DESTINATION libexec/darling/usr/sbin)

#
# BonjourEvents plugin
#
# not currently being built because we have no need for it (i think?), since we don't have UserEventAgent
# besides that, this wouldn't work as is anyways because Darling's `add_bundle` is a little too plain
# (it should generate the proper directory structure to put the bundle in; maybe not by default, but at least as an option)
#add_bundle(BonjourEvents "/usr/libexec/UserEventAgent"
#	mDNSMacOSX/BonjourEvents.c
#)

##
## liblog_mdnsresponder
##
## not currently built because we're missing CoreUtils
##
#add_darling_library(log_mdnsresponder
#	Clients/dnssdutil/DNSMessage.c
#	mDNSMacOSX/LoggingProfiles/liblog_mdnsresponder.m
#)
#make_fat(log_mdnsresponder)
#
#target_link_libraries(log_mdnsresponder PRIVATE
#	system
#)
#
#target_link_options(log_mdnsresponder PRIVATE
#	-fobjc-arc
#	-fobjc-weak
#)
#
#install(TARGETS log_mdnsresponder DESTINATION libexec/darling/usr/lib/log)
