/*	Benjamin DELPY `gentilkiwi`
	http://blog.gentilkiwi.com
	benjamin@gentilkiwi.com
	Licence : http://creativecommons.org/licenses/by/3.0/fr/
*/
#pragma once
#include "globals.h"

typedef PVOID	SAMPR_HANDLE;

typedef enum _USER_INFORMATION_CLASS {
	UserInternal1Information = 18,
	UserAllInformation = 21,
} USER_INFORMATION_CLASS, *PUSER_INFORMATION_CLASS;

typedef struct _SAMPR_SR_SECURITY_DESCRIPTOR {
	DWORD Length;
	PUCHAR SecurityDescriptor;
} SAMPR_SR_SECURITY_DESCRIPTOR, *PSAMPR_SR_SECURITY_DESCRIPTOR;

typedef struct _GROUP_MEMBERSHIP {
	DWORD RelativeId;
	DWORD Attributes;
} GROUP_MEMBERSHIP, *PGROUP_MEMBERSHIP;

typedef struct _SAMPR_USER_INTERNAL1_INFORMATION {
	BYTE NTHash[LM_NTLM_HASH_LENGTH];
	BYTE LMHash[LM_NTLM_HASH_LENGTH];
	BYTE NtPasswordPresent;
	BYTE LmPasswordPresent;
	BYTE PasswordExpired;
	BYTE PrivateDataSensitive;
} SAMPR_USER_INTERNAL1_INFORMATION, *PSAMPR_USER_INTERNAL1_INFORMATION;

typedef union _SAMPR_USER_INFO_BUFFER {
	SAMPR_USER_INTERNAL1_INFORMATION Internal1;
} SAMPR_USER_INFO_BUFFER, *PSAMPR_USER_INFO_BUFFER;

typedef struct _SAMPR_RID_ENUMERATION {
	DWORD RelativeId;
	LSA_UNICODE_STRING Name;
} SAMPR_RID_ENUMERATION, *PSAMPR_RID_ENUMERATION;

typedef struct _SAMPR_GET_MEMBERS_BUFFER {
	DWORD MemberCount;
	DWORD *Members;
	DWORD *Attributes;
} SAMPR_GET_MEMBERS_BUFFER, *PSAMPR_GET_MEMBERS_BUFFER;

extern NTSTATUS WINAPI SamConnect(IN PUNICODE_STRING ServerName, OUT SAMPR_HANDLE * ServerHandle, IN ACCESS_MASK DesiredAccess, IN BOOLEAN Trusted);
extern NTSTATUS WINAPI SamEnumerateDomainsInSamServer(IN SAMPR_HANDLE ServerHandle, OUT DWORD * EnumerationContext, OUT PSAMPR_RID_ENUMERATION* Buffer, IN DWORD PreferedMaximumLength, OUT DWORD * CountReturned);
extern NTSTATUS WINAPI SamLookupDomainInSamServer(IN SAMPR_HANDLE ServerHandle, IN PUNICODE_STRING Name, OUT PSID * DomainId);

extern NTSTATUS WINAPI SamOpenDomain(IN SAMPR_HANDLE SamHandle, IN ACCESS_MASK DesiredAccess, IN PSID DomainId, OUT SAMPR_HANDLE * DomainHandle);
extern NTSTATUS WINAPI SamOpenUser(IN SAMPR_HANDLE DomainHandle, IN ACCESS_MASK DesiredAccess, IN DWORD UserId, OUT SAMPR_HANDLE * UserHandle);
extern NTSTATUS WINAPI SamQueryInformationUser(IN SAMPR_HANDLE UserHandle, IN USER_INFORMATION_CLASS UserInformationClass, PSAMPR_USER_INFO_BUFFER* Buffer);
extern NTSTATUS WINAPI SamGetGroupsForUser(IN SAMPR_HANDLE UserHandle, OUT PGROUP_MEMBERSHIP * Groups, OUT DWORD * CountReturned);
extern NTSTATUS WINAPI SamGetAliasMembership(IN SAMPR_HANDLE DomainHandle, IN DWORD Count, IN PSID * Sid, OUT DWORD * CountReturned, OUT PDWORD * RelativeIds);

extern NTSTATUS WINAPI SamGetMembersInGroup(IN SAMPR_HANDLE GroupHandle, OUT PSAMPR_GET_MEMBERS_BUFFER*Members);


extern NTSTATUS WINAPI SamEnumerateUsersInDomain(IN SAMPR_HANDLE DomainHandle, IN OUT PDWORD EnumerationContext, IN DWORD UserAccountControl, OUT PSAMPR_RID_ENUMERATION* Buffer, IN DWORD PreferedMaximumLength, OUT PDWORD CountReturned);
extern NTSTATUS WINAPI SamLookupNamesInDomain(IN SAMPR_HANDLE DomainHandle, IN DWORD Count, IN PUNICODE_STRING Names, OUT PDWORD * RelativeIds, OUT PDWORD * Use);
extern NTSTATUS WINAPI SamLookupIdsInDomain(IN SAMPR_HANDLE DomainHandle, IN DWORD Count, IN PDWORD RelativeIds, OUT PUNICODE_STRING * Names, OUT PDWORD * Use);
extern NTSTATUS WINAPI SamRidToSid(IN SAMPR_HANDLE ObjectHandle, IN DWORD Rid, OUT PSID * Sid);
extern NTSTATUS WINAPI SamCloseHandle(IN SAMPR_HANDLE SamHandle);
extern NTSTATUS WINAPI SamFreeMemory(IN PVOID Buffer);

#define SAM_SERVER_CONNECT				0x00000001
#define SAM_SERVER_SHUTDOWN				0x00000002
#define SAM_SERVER_INITIALIZE			0x00000004
#define SAM_SERVER_CREATE_DOMAIN		0x00000008
#define SAM_SERVER_ENUMERATE_DOMAINS	0x00000010
#define SAM_SERVER_LOOKUP_DOMAIN		0x00000020
#define SAM_SERVER_ALL_ACCESS			0x000f003f
#define SAM_SERVER_READ					0x00020010
#define SAM_SERVER_WRITE				0x0002000e
#define SAM_SERVER_EXECUTE				0x00020021

#define DOMAIN_READ_PASSWORD_PARAMETERS	0x00000001
#define DOMAIN_WRITE_PASSWORD_PARAMS	0x00000002
#define DOMAIN_READ_OTHER_PARAMETERS	0x00000004
#define DOMAIN_WRITE_OTHER_PARAMETERS	0x00000008
#define DOMAIN_CREATE_USER				0x00000010
#define DOMAIN_CREATE_GROUP				0x00000020
#define DOMAIN_CREATE_ALIAS				0x00000040
#define DOMAIN_GET_ALIAS_MEMBERSHIP		0x00000080
#define DOMAIN_LIST_ACCOUNTS			0x00000100
#define DOMAIN_LOOKUP					0x00000200
#define DOMAIN_ADMINISTER_SERVER		0x00000400
#define DOMAIN_ALL_ACCESS				0x000f07ff
#define DOMAIN_READ						0x00020084
#define DOMAIN_WRITE					0x0002047a
#define DOMAIN_EXECUTE					0x00020301

#define GROUP_READ_INFORMATION			0x00000001
#define GROUP_WRITE_ACCOUNT				0x00000002
#define GROUP_ADD_MEMBER				0x00000004
#define GROUP_REMOVE_MEMBER				0x00000008
#define GROUP_LIST_MEMBERS				0x00000010
#define GROUP_ALL_ACCESS				0x000F001F
#define GROUP_READ						0x00020010
#define GROUP_WRITE						0x0002000E
#define GROUP_EXECUTE					0x00020001

#define ALIAS_ADD_MEMBER				0x00000001
#define ALIAS_REMOVE_MEMBER				0x00000002
#define ALIAS_LIST_MEMBERS				0x00000004
#define ALIAS_READ_INFORMATION			0x00000008
#define ALIAS_WRITE_ACCOUNT				0x00000010
#define ALIAS_ALL_ACCESS				0x000F001F
#define ALIAS_READ						0x00020004
#define ALIAS_WRITE						0x00020013
#define ALIAS_EXECUTE					0x00020008

#define USER_READ_GENERAL				0x00000001
#define USER_READ_PREFERENCES			0x00000002
#define USER_WRITE_PREFERENCES			0x00000004
#define USER_READ_LOGON					0x00000008
#define USER_READ_ACCOUNT				0x00000010
#define USER_WRITE_ACCOUNT				0x00000020
#define USER_CHANGE_PASSWORD			0x00000040
#define USER_FORCE_PASSWORD_CHANGE		0x00000080
#define USER_LIST_GROUPS				0x00000100
#define USER_READ_GROUP_INFORMATION		0x00000200
#define USER_WRITE_GROUP_INFORMATION	0x00000400
#define USER_ALL_ACCESS					0x000f07ff
#define USER_READ						0x0002031a
#define USER_WRITE						0x00020044
#define USER_EXECUTE					0x00020041

#define USER_ALL_USERNAME				0x00000001
#define USER_ALL_FULLNAME				0x00000002
#define USER_ALL_USERID					0x00000004
#define USER_ALL_PRIMARYGROUPID			0x00000008
#define USER_ALL_ADMINCOMMENT			0x00000010
#define USER_ALL_USERCOMMENT			0x00000020
#define USER_ALL_HOMEDIRECTORY			0x00000040
#define USER_ALL_HOMEDIRECTORYDRIVE		0x00000080
#define USER_ALL_SCRIPTPATH				0x00000100
#define USER_ALL_PROFILEPATH			0x00000200
#define USER_ALL_WORKSTATIONS			0x00000400
#define USER_ALL_LASTLOGON				0x00000800
#define USER_ALL_LASTLOGOFF				0x00001000
#define USER_ALL_LOGONHOURS				0x00002000
#define USER_ALL_BADPASSWORDCOUNT		0x00004000
#define USER_ALL_LOGONCOUNT				0x00008000
#define USER_ALL_PASSWORDCANCHANGE		0x00010000
#define USER_ALL_PASSWORDMUSTCHANGE		0x00020000
#define USER_ALL_PASSWORDLASTSET		0x00040000
#define USER_ALL_ACCOUNTEXPIRES			0x00080000
#define USER_ALL_USERACCOUNTCONTROL		0x00100000
#define USER_ALL_PARAMETERS				0x00200000
#define USER_ALL_COUNTRYCODE			0x00400000
#define USER_ALL_CODEPAGE				0x00800000
#define USER_ALL_NTPASSWORDPRESENT		0x01000000
#define USER_ALL_LMPASSWORDPRESENT		0x02000000
#define USER_ALL_PRIVATEDATA			0x04000000
#define USER_ALL_PASSWORDEXPIRED		0x08000000
#define USER_ALL_SECURITYDESCRIPTOR		0x10000000
#define USER_ALL_UNDEFINED_MASK			0xc0000000