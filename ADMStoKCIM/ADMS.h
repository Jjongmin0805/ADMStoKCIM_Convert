#pragma once

#ifndef __KASIM_DEFINE__
#define __KASIM_DEFINE__


// Table : AVM_STA, Type : Online
typedef struct _AVM_STA
{
	wchar_t							avm_nm[64];					// Angle violation monitoring group의 명칭

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} AVM_STA;
#define								AVM_STA_SIZE						sizeof( AVM_STA )


// Table : AVM_DYN_UIN, Type : Online
typedef struct _AVM_DYN_UIN
{
	int								avm_morflag;					// AVM group의 감시 활성 여부(1: 감시, 0: 감시안함)
	double							avm_lm;					// AVM group의 위상차 limit
	double							avm_tol;					// AVM 감시를 위한 tolerance
	double							avm_dband;					// AVM 감시를 위한 dead band

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} AVM_DYN_UIN;
#define								AVM_DYN_UIN_SIZE						sizeof( AVM_DYN_UIN )

//BOF.h 파일에 구조체 생성
typedef struct _BOF_STA
{
	wchar_t							bof_nm[64];																// 지점(Branch Office) 명
	wchar_t							bof_officeid[64];														// Branch Office의 ID
	int								bof_ii_center;															// BOF의 CENTER로의 Indirect index
	int								bof_si_center;															// BOF의 CENTER로의 Sibling index
	int								bof_hi_dl;																// BOF에서 SS로의 head index
	int								bof_AREA_BASE_CODE;

	int								var_reser1;																// 예약 변수1
	int								var_reser2;																// 예약 변수2
	int								var_reser3;																// 예약 변수3

	int								nBOF_OFFICE_TYPE;
} BOF_STA;
#define								BOF_STA_SIZE						sizeof( BOF_STA )


// Table : BR_STA, Type : Online
typedef struct _BR_STA
{
	wchar_t							br_nm[64];					// 분기선로
	int								br_ii_equ;					// BR에서 개별 설비(LNSEC, TR)로의 indirect index
	int								br_si_fnd;					// BR에서 ND로의 sibling index
	int								br_ii_fnd;					// BR에서 ND로의 indirect index
	int								br_si_tnd;					// BR에서 ND로의 sibling index
	int								br_ii_tnd;					// BR에서 ND로의 indirect index
	double							br_posr;					// 정상분 저항
	double							br_posx;					// 정상분 리액턴스
	double							br_posg;					// 정상분 컨덕턴스
	double							br_posb;					// 정상분 서셉턴스
	double							br_zerr;					// 영상분 저항
	double							br_zerx;					// 영상분 리액턴스
	double							br_zerg;					// 영상분 컨덕턴스
	double							br_zerb;					// 영상분 서셉턴스
	double							br_norlm;					// 정상상태 열적용량 제한치
	double							br_emrlm;					// 비상상태 열적용량 제한치
	int								br_si_gbr;					// BR에서 GBR로의 Sibling index
	int								br_ii_gbr;					// BR에서 GBR로의 Indirect index
	int								br_ii_dl;
	int								br_si_dl;
	int								br_ii_equty;

	wchar_t							br_ceq[64];

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} BR_STA;
#define								BR_STA_SIZE						sizeof( BR_STA )


// Table : CBSW_STA, Type : Online
typedef struct _CBSW_STA
{
	wchar_t							cbsw_nm[64];					// 차단기 및 개폐기 등 2_node설비 중 상태변경이 있는 설비의 명칭
	wchar_t							cbsw_ceqid[64];					// CBSW의 CEQID
	int								cbsw_type;					// 스위치 기기 종류(CODE_SWTYPE참조)
	int								cbsw_rtutype;					// 스위치의 RTU 종류(CODE_RTUTYPE참조)
	int								cbsw_RTUCODE;					// 스위치의 RTU 통신여부
	int								cbsw_norstat;					// 스위치의 초기개폐상태 입력
	int								cbsw_ii_prde;
	int								cbsw_si_fnd;					// CBSW에서 From 측 ND로의 sibling index
	int								cbsw_ii_fnd;					// CBSW에서 From 측 ND로의 indirect index
	int								cbsw_si_tnd;					// CBSW에서 To 측 ND로의 sibling index
	int								cbsw_ii_tnd;					// CBSW에서 To 측 ND로의 indirect index
	int								cbsw_si_fgnd;					// CBSW에서 From 측 GND로의 sibling index
	int								cbsw_ii_fgnd;					// CBSW에서 From 측 GND로의 indirect index
	int								cbsw_si_tgnd;					// CBSW에서 To 측 GND로의 sibling index
	int								cbsw_ii_tgnd;					// CBSW에서 To 측 GND로의 indirect index
	int								cbsw_ii_dl;					// Indirect index
	int								cbsw_si_dl;					// Sibling Index
	wchar_t							cbsw_multisw_id[64];					// 다회로 스위치 ID
	int								cbsw_multicir_number;					// 다회로 단자번호
	int								cbsw_comtype;

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} CBSW_STA;
#define								CBSW_STA_SIZE						sizeof( CBSW_STA )


// Table : CBSW_DYN_UIN, Type : Online
typedef struct _CBSW_DYN_UIN
{
	int								cbsw_meause;					// 측정값 사용 여부 표시자(1 : 사용, 0: 사용 안함)
	int								cbsw_manf;					// 수동값 설정 여부 표시자(1 : 수동값 설정, 0: 설정 안함)
	int								cbsw_manstat;					// 수동설정 상태 (1: close, 0: open)
	int								cbsw_dnrimpo;					// NR 연산 제외스위치(연산포함:1,연산제외:2)
	int								cbsw_ctdir;					// CT 극성정보로 Device의 기준 설정(Terminal 정보)과 현재 ACM상의 From/To 노드와의 일치성 여부(일치: 1, 불일치: 2)

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} CBSW_DYN_UIN;
#define								CBSW_DYN_UIN_SIZE						sizeof( CBSW_DYN_UIN )


// Table : CBSW_DYN_MEA, Type : Online
typedef struct _CBSW_DYN_MEA
{
	int								cbsw_odstat;					// 이전 측정 상태 (1: close, 0: open)
	int								cbsw_nwstat;					// 현재 측정 상태 (1: close, 0: open)
	double							cbsw_pameakv;					// From 노드 기준의 A상 전압 KV
	double							cbsw_pbmeakv;					// From 노드 기준의 B상 전압 KV
	double							cbsw_pcmeakv;					// From 노드 기준의 C상 전압 KV
	double							cbsw_pameaamp;					// A상 전류
	double							cbsw_pbmeaamp;					// B상 전류
	double							cbsw_pcmeaamp;					// C상 전류
	double							cbsw_pameaadiff;					// A상 전압-전류 위상차
	double							cbsw_pbmeaadiff;					// B상 전압-전류 위상차
	double							cbsw_pcmeaadiff;					// C상 전압-전류 위상차
	int								cbsw_comstatser;					// Server와의 통신상태(정상 : 1, 비정상 : 0) --> CBSW_COMSTASTDE도 0으로 할당함
	int								cbsw_contstat;					// 스위치 제어가능여(1)/부(0) (통신 불량이 아닌, 제어조작 불가능 상태를 표시)

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} CBSW_DYN_MEA;
#define								CBSW_DYN_MEA_SIZE						sizeof( CBSW_DYN_MEA )


// Table : CENTER_STA, Type : Online
typedef struct _CENTER_STA
{
	wchar_t							center_nm[64];					// 배전센터(Control Center) 명
	wchar_t							center_officeid[64];			// CENTER의 Office ID
	int								center_hi_bof;					// CENTER에서 BOF로의 head index

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3

	int								nCENTER_HEAD_OFFICE_FK;
	int								nCENTER_II_HDOF;
	int								nCENTER_SI_HDOF;
} CENTER_STA;
#define								CENTER_STA_SIZE						sizeof( CENTER_STA )


// Table : DIAINFO_STA, Type : Online
typedef struct _DIAINFO_STA
{
	int								diainfo_ceqtype;					// 계통도 설비 종류
	int								diainfo_ceqindex;					// 계통도 설비 인덱스
	int								diainfo_coordiorder;					// 계통도 멀티라인 좌표 순서
	double							diainfo_ceqax;					// 계통도의 X축 좌표정보
	double							diainfo_ceqay;					// 계통도의 Y축 좌표정보

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} DIAINFO_STA;
#define								DIAINFO_STA_SIZE						sizeof( DIAINFO_STA )


// Table : DL_STA, Type : Online
typedef struct _DL_STA
{
	wchar_t							dl_nm[64];					// 선로(Feeder)명
	wchar_t							dl_distributionlineid[64];					// DistributionLine의 CEQID
	int								dl_ii_olm;					// DL에서 OLM(overload monitoring)으로의 indirect index
	int								dl_ii_mtr;					// DL에서 MTR로의 indirect index
	int								dl_si_mtr;					// DL에서 MTR로의 sibling index
	int								dl_ii_bof;
	int								dl_si_bof;
	int								dl_hi_ij;					// DL에서 IJ로의 Head index
	int								dl_hi_cbsw;					// DL에서 CBSW로의 Head index (변전소 내부 CBSW를 제외)
	int								dl_hi_br;					// DL에서 GBR로의 Head index
	int								dl_ii_cb;

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3

	wchar_t							stDL_MTRFK[64];
	wchar_t							stDL_CEQFK[64];
	wchar_t							stDL_PVLFK[64];
	wchar_t							stDL_SVLFK[64];
	wchar_t							stDL_SUBFK[64];
} DL_STA;
#define								DL_TA_SIZE						sizeof( DL_STA )


// Table : DL_DYN_UIN, Type : Online
typedef struct _DL_DYN_UIN
{
	int								dl_protecseq;					// 해당 DL에서 고려하는 순시보호동작 횟수
	double							dl_cvrfactor;					// 해당 DL별 CVR 수행 시 고려되는 유효전력 감소비율
	double							dl_peaklim;					// 최대 부하(Peak Load) 제한 값
	double							dl_oprcapacity;					// 세팅정보
	int								dl_sca_allflag;
	int								dl_sop_flag;

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} DL_DYN_UIN;
#define								DL_DYN_UIN_SIZE						sizeof( DL_DYN_UIN )


// Table : DNROPT_DYN_UIN, Type : Online
typedef struct _DNROPT_DYN_UIN
{
	int								dnropt_max_iterpfo;					// 조류계산 최대반복횟수
	int								dnropt_max_iterga;					// 유전알고리즘 최대반복횟수
	int								dnropt_nunpop;					// 유전알고리즘 염색체수
	int								dnropt_mutation_rate;					// 유전알고리즘 돌연변이발생 비율 (%)
	double							dnropt_ratio_lb;					// 복합 목적함수에서 손실의비율(0.1~0.9)
	int								dnropt_cben;					// NR에 CB의 참여여부
	int								dnropt_reen;					// NR에 RECLOSER의 참여여부
	int								dnropt_gaen;					// NR에 자동화 스위치의 참여여부
	int								dnropt_maen;					// NR에 수동 스위치의 참여여부
	int								dnropt_object;					// NR의 목적함수(1.손실저감 2.피더밸런스 3,복합목적함수)
	int								dnropt_max_chgsw;					// 최대 허용 스위치 변동수 (0:제한없음, 1~:스위치 제한수)
	int								dnropt_penalty_chgsw;					// 허용 스위치 변동량 초과분에 대한 패널티율
	int								dnropt_penalty_volt;					// 전압 제한치 초과분에 대한 패널티율
	int								dnropt_penalty_mw;					// 용량제한치 초과분에 대한 패널티율

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} DNROPT_DYN_UIN;
#define								DNROPT_DYN_UIN_SIZE						sizeof( DNROPT_DYN_UIN )


// Table : EQUTY_STA, Type : Online
typedef struct _EQUTY_STA
{
	wchar_t							equty_nm[64];					// 설비종류
	int								equty_hi_gbr;					// EQUTY에서 GBR로의 head index
	int								equty_hi_ij;					// EQUTY에서 IJ로의 head index

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} EQUTY_STA;
#define								EQUTY_STA_SIZE						sizeof( EQUTY_STA )


// Table : ESCOPT_DYN_UIN, Type : Online
typedef struct _ESCOPT_DYN_UIN
{
	double							escopt_smwprp;					// 시스템 피크 reduction price
	double							escopt_dmwprp;					// MG 피크 reduction price
	int								escopt_fsoctm;					// Full SOC 제약 시간

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} ESCOPT_DYN_UIN;
#define								ESCOPT_DYN_UIN_SIZE						sizeof( ESCOPT_DYN_UIN )


// Table : ESS_STA, Type : Online
typedef struct _ESS_STA
{
	wchar_t							ess_nm[64];					// Energy Storage System(ESS) 명칭
	int								ess_chgtm;					// ESS 충전가능 시간
	int								ess_dchgtm;					// ESS 방전가능 시간
	int								ess_ii_gen;
	int								ess_si_gen;
	int								ess_type;					// ESS의 Battery Type
	double							ess_charmxmw;					// 충전시 최대출력값
	double							ess_charmnmw;					// 충전시 최소출력값
	double							ess_dichmxmw;					// 방전시 최대출력값
	double							ess_dichmnmw;					// 방전시 최소출력값
	double							ess_mxsoc;					// 제조사에서 제공하는 운전SOC 상한값
	double							ess_mnsoc;					// 제조사에서 제공하는 운전SOC 하한값
	double							ess_chgeff;					// 충전 효율
	double							ess_c_ratio;					// ESS의 C_RATIO
	int								ess_hi_pcs;					// ESS에서 PCS로의 Head Index
	int								ess_noofpcs;					// ESS에 소속된 PCS 개수
	double							ess_capmwh;					// ESS 최대 용량

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} ESS_STA;
#define								ESS_STA_SIZE						sizeof( ESS_STA )


// Table : ESS_DYN_UIN, Type : Online
typedef struct _ESS_DYN_UIN
{
	double							ess_setmxsoc;					// 배터리 운전SOC 상한값
	double							ess_setmnsoc;					// 배터리 운전SOC 하한값
	double							ess_efin;					// ESS의 최종 에너지 목표값 (SOC 유지 목표치)
	double							ess_clrmxsoc;					// SOC 상한 위배로 비상운전시 SOC 회복 정상운전 판단 기준
	double							ess_clrmnsoc;					// SOC 하한 위배로 비상운전시 SOC 회복 정상운전 판단 기준
	int								ess_cntmode;					// 제어 모드 (CVCF/TARGET)
	double							ess_socmxdband;					// 최대 SOC 기준 범위 이상으로 SOC가 넘어간 경우 방전해야 하는 전력량을 정하기 위한 기준
	double							ess_socmndband;					// 최소 SOC 기준 범위 이하로 SOC가 내려간 경우 충전해야 하는 전력량을 정하기 위한 기준
	int								ess_soc_ncount_max;					// ESS의 SOC가 일정시간동안 변동하지 않을 때 이상 발생 여부를 판단하는 기준
	double							ess_soc_dev_absolute_max;					// ESS의 SOC 변동량의 절대값 크기의 한계 기준값
	int								ess_mode;					// PMS의 운전 자동/수동 설정 Flag(0: 수동, 1: 자동)
	double							ess_gensche[192];					// PMS의 수동 설정 출력 스케쥴
	int								ess_esschgmode[192];					// PMS의 수동설정 충/방전 모드  스케쥴
	double							ess_opp;					// ESS 운전 비용
	int								ess_outage_time;					// 시간

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} ESS_DYN_UIN;
#define								ESS_DYN_UIN_SIZE						sizeof( ESS_DYN_UIN )


// Table : GBR_STA, Type : Online
typedef struct _GBR_STA
{
	wchar_t							gbr_nm[64];					// 분기선로
	int								gbr_ii_equ;					// GBR에서 개별 설비(LNSEC, TR)로의 indirect index
	int								gbr_si_fgnd;					// GBR에서 GND로의 sibling index
	int								gbr_ii_fgnd;					// GBR에서 GND로의 indirect index
	int								gbr_si_tgnd;					// GBR에서 GND로의 sibling index
	int								gbr_ii_tgnd;					// GBR에서 GND로의 indirect index
	double							gbr_posr;					// 정상분 저항
	double							gbr_posx;					// 정상분 리액턴스
	double							gbr_posg;					// 정상분 컨덕턴스
	double							gbr_posb;					// 정상분 서셉턴스
	double							gbr_zerr;					// 영상분 저항
	double							gbr_zerx;					// 영상분 리액턴스
	double							gbr_zerg;					// 영상분 컨덕턴스
	double							gbr_zerb;					// 영상분 서셉턴스
	int								gbr_hi_br;					// GBR에서 BR로의 Head Index
	int								gbr_si_equty;					// GBR에서 EQUTY로의 Sibling Index
	int								gbr_ii_equty;					// GBR에서 EQUTY로의 Indirect Index
	int								gbr_ii_dl;					// DL로의 Indirect index
	int								gbr_si_dl;					// DL로의 Sibling index

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} GBR_STA;
#define								GBR_STA_SIZE						sizeof( GBR_STA )


// Table : GBR_DYN_UIN, Type : Online
typedef struct _GBR_DYN_UIN
{
	int								gbr_morflag;					// 개별 선로/변압기의 과부하 감시 여부(1: 감시, 0: 감시안함)
	int								gbr_scaflag;					// 고장계산시 branch 전류 계산에 포함할 것인지 여부(1: 포함, 0: 포함안함)

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} GBR_DYN_UIN;
#define								GBR_DYN_UIN_SIZE						sizeof( GBR_DYN_UIN )

// Table : GEN_STA, Type : Online
typedef struct _GENUNIT_STA
{
	wchar_t							GENUNIT_CEQID[64];					// 발전기 명
	wchar_t							GENUNIT_NM[128];					// 발전기 명
	int								GENUNIT_II_GEN;					// GEN에서 ND로의 sibling index
	int								GENUNIT_SI_GEN;					// GEN에서 ND로의 sibling index
	double							GENUNIT_CAP_KW;					// GEN에서 ND로의 sibling index
	wchar_t							GENUNIT_LOCATION_NO[64];						// 발전기 CEQ ID
	wchar_t							GENUNIT_LOCATION_NM[64];						// 발전기 CEQ ID
	wchar_t							GENUNIT_CUSTOMER_NO[64];						// 발전기 CEQ ID.
	int								GENUNIT_TYPE;					// GEN에서 ND로의 sibling index
	wchar_t							GENUNIT_II_EQU_ID[64];					// 20220809
	int								GENUNIT_II_NAME_TYPE_FK;
	int								GENUNIT_INDEX;
	int								GENUNIT_CONNECTION_TR_TYPE;


	int								GENUNIT_LVRTXGROUP_FLAG;
	int								GENUNIT_PUBXGROUP_FLAG;
	int								GENUNIT_PUBOGROUP_FLAG;
	int								GENUNIT_CACGROUP_FLAG;
	int								GENUNIT_MANOGROUP_FLAG;
	int								GENUNIT_MANXGROUP_FLAG;

} GENUNIT_STA;
#define								GENUNIT_STA_SIZE						sizeof( GENUNIT_STA )

// Table : GEN_STA, Type : Online
typedef struct _GEN_STA
{
	wchar_t							gen_nm[64];					// 발전기 명
	wchar_t							gen_ceqid[64];						// 발전기 CEQ ID
	int								gen_si_nd;					// GEN에서 ND로의 sibling index
	int								gen_ii_nd;					// GEN에서 ND로의 indirect index
	int								gen_si_gnd;					// GEN에서 GND로의 sibling index
	int								gen_ii_gnd;					// GEN에서 GND로의 indirect index
	int								gen_ii_ij;					// GEN에서 IJ로의 indirect index
	int								GEN_SI_LNSEC;					// GEN에서 GND로의 indirect index
	int								GEN_II_LNSEC;					// GEN에서 IJ로의 indirect index
	double							gen_namkv;					// Gen의 기기 명판전압
	double							gen_mwlmmx;					// 최대 MW 발전용량(설비용량 기준)
	double							gen_mwlmmn;					// 최소 MW 발전용량(설비용량 기준)
	double							gen_mvarlmmx;					// 최대 MVAR 제한치(설비용량 기준)
	double							gen_mvarlmmn;					// 최소 MVAR 제한치(설비용량 기준)
	double							gen_r;					// 발전기 저항
	double							gen_stx;					// 발전기 차과도 리액턴스
	double							gen_ssx;					// 발전기 정상 리액턴스
	int								gen_type;					// 발전기 종류 (1:무한모선, 2: 디젤, 3:ESS, 4:풍력, 5:태양광, 6:저압연계, 7: 고압연계)
	int								gen_vol_cls;
	int								gen_ii_connd;					// control node로의 indriect index
	int								gen_conndrep;					// control node의 측정값중 기준 상전압(1:A상,2:B상,3:C상,4:AB,5:BC,6:CA)
	int								gen_contype;					// 분산전원 연결 정보 (1:A 상, 2:B상, 3:C상, 4:AB상, 5:BC상, 6:CA상, 7:3상)
	int								gen_noofp;					// 극수 (defualt 4)
	double							gen_pf;					// 역률 (default 0.85)
	double							gen_eff;					// 효율 (default 0.85)
	double							gen_pfminlag;					// 발전기 기기적 최소 지상 역률
	double							gen_pfminlead;					// 발전기 기기적 최소 진상 역률
	int								gen_ii_ess;					// Gen에서 ESS로의 Indrect Index
	int								gen_mea;					// 개별 측정 여부(0: 없음, 1: 있음)
	int								gen_avr;					// 자동전압 제어 참여여부 (1: 참여, 0: 참여안함)
	int								GEN_II_PRDE;
	int								GEN_TREXCL;

	double							fGEN_ITR_CAP;
	double							fGEN_ITR_R;
	double							fGEN_ITR_X;
	double							fGEN_ITR_NGR_R;
	double							fGEN_ITR_NGR_X;
	int								nGEN_ITR_WDC;
	int								nGEN_MACH_TYPE;

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} GEN_STA;
#define								GEN_STA_SIZE						sizeof( GEN_STA )


// Table : GEN_DYN_UIN, Type : Online
typedef struct _GEN_DYN_UIN
{
//	int								gen_avr;					// 자동전압 제어 참여여부 (1: 참여, 0: 참여안함)
	double							gen_tbsvl;					// 제어 모선의 전압 목표치
	double							gen_dev;					// 제어 모선 A상의 전압 목표치 편차
	double							gen_pf;					// 역률제어를 위한 역률 목표치
	int								gen_vvoflag;					// 발전기 무효전력 제어에 VVO 참여여부(0: 불참, 1: 참여)
	int								gen_mvarconmode;					// 무효전력 제어모드(1:전압, 2:역률, 3: 일정무효전력)
	double							gen_outq;					// 일정 무효전력 제어 모드인 발전기의 무효전력 목표치
	double							gen_pfvvominlag;					// VVO 제어시 발전기 최소 지상 역률
	double							gen_pfvvominlead;					// VVO 제어시 발전기 최소 진상 역률


	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} GEN_DYN_UIN;
#define								GEN_DYN_UIN_SIZE						sizeof( GEN_DYN_UIN )


// Table : GND_STA, Type : Online
typedef struct _GND_STA
{
	wchar_t							gnd_nm[64];					// Group 노드명
	int								gnd_hi_nd;					// Group ND에서 ND로의 Head index
	int								gnd_hi_fcbsw;					// GND에서 From 측 CBSW로의 head index
	int								gnd_hi_tcbsw;					// GND에서 To측 CBSW로의 head index
	int								gnd_hi_fgbr;					// GND에서 From측 GBR로의 head index
	int								gnd_hi_tgbr;					// GND에서 To측 GBR로의 head index
	int								gnd_hi_gen;					// GND에서 GEN로의 head index
	int								gnd_hi_shunteq;					// GND에서 SHUNTEQ로의 head index
	int								gnd_hi_svc;					// GND에서 SVC로의 head index
	int								gnd_hi_ld;					// GND에서 LD로의 head index
	int								gnd_hi_ij;					// GND에서 IJ로의 head index
	int								gnd_hi_hvcus;

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3

	int								nGND_SS;
} GND_STA;
#define								GND_STA_SIZE						sizeof( GND_STA )


// Table : IJ_STA, Type : Online
typedef struct _IJ_STA
{
	wchar_t							ij_nm[64];					// Injection 명
	int								ij_si_equty;					// IJ에서 EQUTY로의 sibling index
	int								ij_ii_equty;					// IJ에서 EQUTY로의 indirect index
	int								ij_ii_equ;					// IJ에서 개별 설비로의 indirect index
	int								ij_si_gnd;					// IJ에서 GND로의 sibling index
	int								ij_ii_gnd;					// IJ에서 GND로의 indirect index
	int								ij_ii_dl;					// DL로의 Indirect index
	int								ij_si_dl;					// DL로의 Sibling index

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} IJ_STA;
#define								IJ_STA_SIZE						sizeof( IJ_STA )


// Table : IJ_STA, Type : Online
typedef struct _IJ_DYN_UIN
{
	int								ij_socmaxorder;					// SOC Max 기준 초과시 제어순서
	int								ij_socminorder;					// SOC Min 기준 초과시 제어순서

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} IJ_DYN_UIN;
#define								IJ_DYN_UIN_SIZE					sizeof( IJ_DYN_UIN )


// Table : IJ_DYN_MEA, Type : Online
typedef struct _IJ_DYN_MEA
{
	double							ij_3pmeamw;					// 3상 일괄의 MW 측정값
	double							ij_3pmeamvar;					// 3상 일괄의 MVAR 측정값
	double							ij_pameamw;					// A상의  MW 측정값
	double							ij_pameamvar;					// A상의 MVAR 측정값
	double							ij_pbmeamw;					// B상의  MW 측정값
	double							ij_pbmeamvar;					// B상의 MVAR 측정값
	double							ij_pcmeamw;					// C상의  MW 측정값
	double							ij_pcmeamvar;					// C상의 MVAR 측정값
	int								ij_meaflag;					// AMI Gateway 측정치 유무(1: 측정, 0: 없음 또는 부분측정)

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} IJ_DYN_MEA;
#define								IJ_DYN_MEA_SIZE						sizeof( IJ_DYN_MEA )


// Table : INNERPOLE_STA, Type : Online
typedef struct _INNERPOLE_STA
{
	wchar_t							innerpole_nm[64];					// 전주명 (지상설비 포함)
	int								innerpole_ii_lnsec;					// INNERPole 중 LNSEC의 from측 전주의 Head Index
	int								innerpole_si_lnsec;					// INNERPole 중 LNSEC의 to측 전주의 Head Index
	double							innerpole_xcoodi;					// 전주의 계통도(or GIS)의 X축 좌표정보
	double							innerpole_ycoodi;					// 전주의 계통도(or GIS)의 Y축 좌표정보
	//20200320
	int								innerpole_patrtap;
	int								innerpole_pbtrtap;
	int								innerpole_pctrtap;
	wchar_t							innerpole_comnm[64];
	int								innerpole_numcustom;

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} INNERPOLE_STA;
#define								INNERPOLE_STA_SIZE						sizeof( INNERPOLE_STA )


// Table : INNERSEC_STA, Type : Online
typedef struct _INNERSEC_STA
{
	wchar_t							innersec_nm[64];					// 선로명
	int								innersec_ndissysid;					// NDIS의 선로(HVLINE)의 SYS ID
	int								innersec_ii_lnsec;					// INNERSEC 중 LNSEC으로의 Indirect Index
	int								innersec_si_lnsec;					// INNERSEC 중 LNSEC으로의 sibling Index
	int								innersec_linetype;					// 선종

	int								innersec_hvl_seq;
	double							innersec_fx;
	double							innersec_fy;
	double							innersec_tx;
	double							innersec_ty;

	int							innersec_id;


	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} INNERSEC_STA;
#define								INNERSEC_STA_SIZE						sizeof( INNERSEC_STA )


// Table : LD_STA, Type : Online
typedef struct _LD_STA
{
	wchar_t							ld_nm[64];					// 구간 부하명
	int								ld_si_gnd;					// LD에서 GND로의 sibling index
	int								ld_ii_gnd;					// LD에서 GND로의 indirect index
	int								ld_ii_ij;					// LD에서 IJ로의 indirect index

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} LD_STA;
#define								LD_STA_SIZE						sizeof( LD_STA )


// Table : LD_DYN_UIN, Type : Online
typedef struct _LD_DYN_UIN
{
	double							ld_div;					// 구간 부하 분할계수

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} LD_DYN_UIN;
#define								LD_DYN_UIN_SIZE						sizeof( LD_DYN_UIN )


// Table : LNSEC_STA, Type : Online
typedef struct _LNSEC_STA
{
	int								lnsec_sta_id;
	 
	wchar_t							lnsec_nm[64];					// 선로구간
	wchar_t							lnsec_ceqid[64];					// ACLineSegment의 CEQID
	int								lnsec_ii_br;					// LNSEC에서 BR로의 indirect index
	int								lnsec_hi_innerpole;					// LNSEC에서 INNERPOLE로의 HEAD index
	int								lnsec_hi_fpole;					// LNSEC에서 from쪽 POLE로의 HEAD index
	int								lnsec_hi_tpole;					// LNSEC에서 to쪽 POLE로의 HEAD index
	int								lnsec_hi_innersec;					// LNSEC에서 INNERSEC로의 HEAD index
	int								LNSEC_HI_GEN;					// LNSEC에서 INNERSEC로의 HEAD index
	int								lnsec_privat;					// 선로의 전용선로 여부(1:전용선로, 0:일반선로)
	int								lnsec_type_id;					// KASIM_CODE_LINETYPE참조
	double							lnsec_length;					// 선로 긍장(km)
	int								lnsec_constype;					// 선로 지공/가공 구분


	int								lnsec_SI_FND;					// 선로 지공/가공 구분
	int								lnsec_II_FND;					// 선로 지공/가공 구분
	int								lnsec_SI_TND;					// 선로 지공/가공 구분
	int								lnsec_II_TND;					// 선로 지공/가공 구분


	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} LNSEC_STA;
#define								LNSEC_STA_SIZE						sizeof( LNSEC_STA )


// Table : MTR_STA, Type : Online
typedef struct _MTR_STA
{
	wchar_t							mtr_nm[64];					// 주변압기(Group 개념임) 명
	wchar_t							mtr_maintrid[64];					// MainTransformer의 CEQID
	int								mtr_ii_ss;					// MTR에서 SS로의 indirect index
	int								mtr_si_ss;					// MTR에서 SS로의 sibling index
	int								mtr_hi_dl;					// MTR에서 DL로의 head index

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3

	wchar_t							stMTR_PRIMARY_VOLTAGELEVEL[64];
	wchar_t							stMTR_SECONDARY_VOLTAGELEVEL[64];

	int								mtr_hi_tr;					// MTR에서 DL로의 head index
	int								mtr_bank;					// MTR에서 DL로의 head index

} MTR_STA;
#define								MTR_STA_SIZE						sizeof( MTR_STA )

typedef struct _MTR_DYN_UIN
{
	int								mtr_sop_flag;					// SOP생성대상
	int								mtr_sca_allflag;				// 모든모선 고장전류 생성대성

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} MTR_DYN_UIN;
#define								MTR_DYN_UIN_SIZE						sizeof( MTR_DYN_UIN )


// Table : NCPOPT_DYN_UIN, Type : Online
typedef struct _NCPOPT_DYN_UIN
{
	int								ncpopt_islbs;					// 유효 독립계통 최소 모선수
	int								ncpopt_islgen;					// 유효 독립계통 최소 발전기수
	int								ncpopt_islld;					// 유효 독립계통 최소 부하수
	int								ncpopt_init;					// NCP Initial Mode 수행 여부 Flag(1: 강제수행, 0: 수행안함)

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} NCPOPT_DYN_UIN;
#define								NCPOPT_DYN_UIN_SIZE						sizeof( NCPOPT_DYN_UIN )


// Table : ND_STA, Type : Online
typedef struct _ND_STA
{
	wchar_t							nd_nm[64];					// 노드명
	wchar_t							nd_ceqid[64];					// BusbarSection의 CEQID
	wchar_t							nd_connectivitynodeid[64];					// BusbarSection의 CEQID
	int								nd_si_snv;					// ND에서 SNV로의 sibling index
	int								nd_ii_snv;					// ND에서 SNV로의 indirect index
	int								nd_hi_fcbsw;					// ND에서 From 측 CBSW로의 head index
	int								nd_hi_tcbsw;					// ND에서 To측 CBSW로의 head index
	int								nd_hi_fbr;					// ND에서 From측 BR로의 head index
	int								nd_hi_tbr;					// ND에서 To측 BR로의 head index
	int								nd_hi_gen;					// ND에서 GEN로의 head index
	int								nd_hi_shunteq;					// ND에서 SHUNTEQ로의 head index
	int								nd_hi_svc;					// ND에서 SVC로의 head index
	int								nd_ii_gnd;					// ND에서 GND로의 Indirect Index
	int								nd_si_gnd;					// ND에서 GND로의 Sibling Index

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3

	int								nND_DL;
	int								nND_SS;
} ND_STA;
#define								ND_STA_SIZE						sizeof( ND_STA )


// Table : ND_DYN_UIN, Type : Online
typedef struct _ND_DYN_UIN
{
	int								nd_morflag;					// Node의 전압감시 여부(1: 감시, 0: 감시안함)
	int								nd_ii_vvm;					// ND에서 VVM(voltage violation monitoring)으로의 indirect index

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} ND_DYN_UIN;
#define								ND_DYN_UIN_SIZE						sizeof( ND_DYN_UIN )


// Table : OLM_STA, Type : Online
typedef struct _OLM_STA
{
	wchar_t							olm_nm[64];					// Overload monitoring group의 명칭

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} OLM_STA;
#define								OLM_STA_SIZE						sizeof( OLM_STA )


// Table : OLM_DYN_UIN, Type : Online
typedef struct _OLM_DYN_UIN
{
	int								olm_morflag;					// OLM group의 감시 활성 여부(1: 감시, 0: 감시안함)
	double							olm_tol;					// OLM 감시를 위한 tolerance
	double							olm_dband;					// OLM 감시를 위한 dead band

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} OLM_DYN_UIN;
#define								OLM_DYN_UIN_SIZE						sizeof( OLM_DYN_UIN )


// Table : PFVM_STA, Type : Online
typedef struct _PFVM_STA
{
	wchar_t							pfvm_nm[64];					// Power Factor violation monitoring group의 명칭

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} PFVM_STA;
#define								PFVM_STA_SIZE						sizeof( PFVM_STA )


// Table : PFVM_DYN_UIN, Type : Online
typedef struct _PFVM_DYN_UIN
{
	int								pfvm_morflag;					// PFVM group의 감시 활성 여부(1: 감시, 0: 감시안함)
	double							pfvm_lm;					// PFVM group의 역율 limit
	double							pfvm_tol;					// PFVM 감시를 위한 tolerance
	double							pfvm_dband;					// PFVM 감시를 위한 dead band

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} PFVM_DYN_UIN;
#define								PFVM_DYN_UIN_SIZE						sizeof( PFVM_DYN_UIN )


// Table : POLE_STA, Type : Online
typedef struct _POLE_STA
{
	wchar_t							pole_nm[64];					// 전주명 (지상설비 포함)
	wchar_t							pole_comnm[64];					// 전주전산화 번호


	int								pole_sysid;						// Pole에서 INNERPOLE로의 indirect index
	int								pole_ii_innerpole;				// Pole에서 INNERPOLE로의 indirect index
	int								pole_ptrflag;					// Pole에 연결된 변압기 유무(1:있음, 0:없음)
	int								pole_numcustom;					// 연결된 고객호수

	wchar_t							pole_SYSID_ID[64];
	wchar_t							pole_MRID[64];
	int								pole_nseq;
	double							pole_f1EQx;
	double							pole_f1EQy;

	double							pole_f2EQx;
	double							pole_f2EQy;

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} POLE_STA;
#define								POLE_STA_SIZE						sizeof( POLE_STA )


// Table : RPFOPT_DYN_UIN, Type : Online
typedef struct _RPFOPT_DYN_UIN
{
	int								rpfopt_flat;					// Flat start 여부
	int								rpfopt_max_iterations;					// 최대 반복횟수
	int								rpfopt_svc_adj_limit;					// SVC 조정횟수
	double							rpfopt_island_max_voltage_limit;					// 허용전압 최대
	double							rpfopt_island_min_voltage_limit;					// 허용전압 최소
	double							rpfopt_near_convergence_angle_limit;					// 부분수렴 기준(위상)
	double							rpfopt_near_convergence_voltage_limit;					// 부분수렴 기준(전압)
	double							rpfopt_near_convergence_mw_limit;					// 부분수렴 기준(MW)
	double							rpfopt_near_convergence_mvar_limit;					// 부분수렴 기준(MVAr)
	double							rpfopt_convergence_angle_limit;					// 완전수렴 기준(위상)
	double							rpfopt_convergence_voltage_limit;					// 완전수렴 기준(전압)
	double							rpfopt_convergence_mw_limit;					// 완전수렴 기준(MW)
	double							rpfopt_convergence_mvar_limit;					// 완전수렴 기준(MVAr)
	double							rpfopt_largest_mismatch_limit;					// 최대 Mismatch

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} RPFOPT_DYN_UIN;
#define								RPFOPT_DYN_UIN_SIZE						sizeof( RPFOPT_DYN_UIN )


// Table : SCAOPT_DYN_UIN, Type : Online
typedef struct _SCAOPT_DYN_UIN
{
	double							scaopt_fltz;					// 고장점 임피던스
	int								scaopt_basemva;					// 기준 용량
	int								scaopt_basefreq;					// 기준 주파수
	int								scaopt_flttype_3p;					// 3상고장 계산 여부 Flag (1: 계산함, 0: 계산안함)
	int								scaopt_flttype_2p;					// 선간단락고장 계산 여부 Flag (1: 계산함, 0: 계산안함)
	int								scaopt_flttype_2g;					// 2선지락고장 계산 여부 Flag (1: 계산함, 0: 계산안함)
	int								scaopt_flttype_1g;					// 1선지락고장 계산 여부 Flag (1: 계산함, 0: 계산안함)
	int								scaopt_flttype_p1cir;					// 1상계통 고장 계산 여부 Flag (1: 계산함, 0: 계산안함)
	int								scaopt_prev;					// 고장계산시 전압 적용 (0: Flat, 1: 조류계산결과)
	int								scaopt_allflag;					// 모든 모선에 대한 고장계산 여부(0: 계산안함, 1: 게산함)
	int								scaopt_manbs;					// 고장계산 모선 번호
	int								scaopt_halfflag;					// 1/2 cycle의 고장계산 여부
	int								scaopt_fiveflag;					// 5 cycle의 고장계산 여부
	int								scaopt_steadyflag;					// Steady state의 고장계산 여부

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} SCAOPT_DYN_UIN;
#define								SCAOPT_DYN_UIN_SIZE						sizeof( SCAOPT_DYN_UIN )


// Table : SHUNTEQ_STA, Type : Online
typedef struct _SHUNTEQ_STA
{
	wchar_t							shunteq_nm[64];					// 병렬 장치
	wchar_t							shunteq_ceqid[64];				// SHUNTEQ의 CEQID
	int								shunteq_si_nd;					// SUNHTEQ에서 ND로의 sibling index
	int								shunteq_ii_nd;					// SUNHTEQ에서 ND로의 indirect index
	int								shunteq_si_gnd;					// SUNHTEQ에서 GND로의 sibling index
	int								shunteq_ii_gnd;					// SUNHTEQ에서 GND로의 indirect index
	int								shunteq_ii_ij;					// SUNHTEQ에서 IJ로의 indirect index
	int								shunteq_type;					// Shunt device 종류(1: Shunt Capacitor, 2: Shunt Reactor, 3: 기타)
	double							shunteq_mvar;					// Shunt device의 MVAr 용량
	int								shunteq_ii_connd;					// Control node로의 indriect index
	int								shunteq_repphs;					// control node의 측정값중 기준 상전압(1:A상,2:B상,3:C상,4:AB,5:BC,6:CA)

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} SHUNTEQ_STA;
#define								SHUNTEQ_STA_SIZE						sizeof( SHUNTEQ_STA )


// Table : SHUNTEQ_DYN_UIN, Type : Online
typedef struct _SHUNTEQ_DYN_UIN
{
	int								shunteq_avr;					// 자동전압 제어 참여여부 (1: 참여, 0: 참여안함)
	double							shunteq_tbsvl;					// 제어 모선의 전압 목표치
	double							shunteq_dev;					// 제어 모선의 전압 목표치 편차
	int								shunteq_vvoflag;					// Active Voltage Var control 제어에 참여할 것인지 여부
	double							shunteq_shcost;					// Shunt Equipment 가격
	int								shunteq_opmx;					// 최대 상태변경 횟수
	int								shunteq_conmxday;					// 하루 최대 동작횟수
	int								shunteq_opday;					// 하루 동작횟수

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} SHUNTEQ_DYN_UIN;
#define								SHUNTEQ_DYN_UIN_SIZE						sizeof( SHUNTEQ_DYN_UIN )


// Table : SNV_STA, Type : Online
typedef struct _SNV_STA
{
	wchar_t							snv_nm[64];					// 변전소 공칭전압 명
	double							snv_norkv;					// 공칭전압 kV
	int								snv_si_ss;					// SNV에서 SS로의 sibling index
	int								snv_ii_ss;					// SNV에서 SS로의 indirect index
	int								snv_hi_nd;					// SNV에서 ND로의 head index

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} SNV_STA;
#define								SNV_STA_SIZE						sizeof( SNV_STA )


// Table : SS_STA, Type : Online
typedef struct _SS_STA
{
	wchar_t							ss_nm[64];					// 변전소 명
	wchar_t							ss_substationid[64];		// SubStation의 CEQID
	wchar_t							ss_code[64];				// SubStation의 CEQID
	int								ss_ii_bof;					// SS에서 BOF로의 indirect index
	int								ss_si_bof;					// SS에서 BOF로의 sibling index
	int								ss_hi_mtr;					// SS에서 MTR로의 head index
	int								ss_hi_snv;					// SS에서 SNV로의 head index

	int								ss_nd;
	int								ss_ii_vgen;

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} SS_STA;
#define								SS_STA_SIZE						sizeof( SS_STA )


// Table : SS_DYN_UIN, Type : Online
typedef struct _SS_DYN_UIN
{
	int								ss_dnrflag;					// 해당 SS가 DNR의 연산영역인지 구분자(1: 연산 포함, 0: 연산제외)
	double							ss_tieoutkw;					// 연계선로 송출조류 한계값
	double							ss_tieinkw;					// 연계선로 인입조류 한계값

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} SS_DYN_UIN;
#define								SS_DYN_UIN_SIZE						sizeof( SS_DYN_UIN )


// Table : SVC_STA, Type : Online
typedef struct _SVC_STA
{
	wchar_t							svc_nm[64];					// SVC 명
	wchar_t							svc_ceqid[64];				// SVC의 CEQID
	int								svc_si_nd;					// SVC에서 ND로의 sibling index
	int								svc_ii_nd;					// SVC에서 ND로의 indirect index
	int								svc_si_gnd;					// SVC에서 GND로의 sibling index
	int								svc_ii_gnd;					// SVC에서 GND로의 indirect index
	int								svc_ii_ij;					// SVC에서 IJ로의 indirect index
	double							svc_shmvar;					// SVC의 무효전력용량(MVAR)
	int								svc_bano;					// SVC의 BANO
	double							svc_mvarlmmn;					// SVC 조정가능 최소 MVAR제한용량
	double							svc_mvarlmmx;					// SVC 조정가능 최대 MVAR제한용량
	int								svc_ii_connd;					// 제어노드
	int								svc_conndrep;					// control node의 측정값중 기준 상전압(1:A상,2:B상,3:C상,4:AB,5:BC,6:CA)

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} SVC_STA;
#define								SVC_STA_SIZE						sizeof( SVC_STA )


// Table : SVC_DYN_UIN, Type : Online
typedef struct _SVC_DYN_UIN
{
	double							svc_tarvol;					// SVC의 목표전압값
	double							svc_slop;					// SVC Sloop 특성
	int								svc_vvoflag;					// SVC의 VVO 참여여부(1:참여, 0:불참여)

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} SVC_DYN_UIN;
#define								SVC_DYN_UIN_SIZE						sizeof( SVC_DYN_UIN )


// Table : TR_STA, Type : Online
typedef struct _TR_STA
{
	wchar_t							tr_nm[64];					// 변압기기
	wchar_t							tr_ceqid[64];					// 변압기 CEQID
	int								tr_type;					// 2,3권선, SVR 구분자 (1: 2권선 MTR, 2: 3권선 MTR, 3: SVR, 4: 일반 2권선, 5: 일반 3권선)
	int								tr_ii_br;					// TR에서 GBR로의 indirect index
	double							tr_fnorkv;					// From 측 공칭전압
	double							tr_tnorkv;					// To 측 공칭전압	
	double							tr_trmva;					// TR 용량
	int								tr_conty;					// 결선방식 (0: From, To 모두 비접지, 1: From 쪽 접지, 2: To쪽 접지, 3: From, To 모두 접지)
	int								tr_onltc;					// OLTC/NLTC 구분자 (0: OLTC, 1: NLTC)
	double							tr_posx;					// 정상분 리액턴스
	double							tr_zerx;					// 영상분 리액턴스
	int								tr_patapmx;					// A상 Tap 설정 최대치 (상별 구분이 없는 경우 동일값 사용)
	int								tr_patapmn;					// A상 Tap 설정 최소치 (상별 구분이 없는 경우 동일값 사용)
	int								tr_patapnor;					// A상 공칭 Tap 위치 (상별 구분이 없는 경우 동일값 사용)
	double							tr_patapstep;					// A상 Tap position 간 변화량 (상별 구분이 없는 경우 동일값 사용)
	int								tr_pbtapmx;					// B상 Tap 설정 최대치 (상별 구분이 없는 경우 동일값 사용)
	
	int								tr_pbtapmn;					// B상 Tap 설정 최소치 (상별 구분이 없는 경우 동일값 사용)
	int								tr_pbtapnor;					// B상 공칭 Tap 위치 (상별 구분이 없는 경우 동일값 사용)
	double							tr_pbtapstep;					// B상 Tap position 간 변화량 (상별 구분이 없는 경우 동일값 사용)
	int								tr_pctapmx;					// C상 Tap 설정 최대치 (상별 구분이 없는 경우 동일값 사용)
	int								tr_pctapmn;					// C상 Tap 설정 최소치 (상별 구분이 없는 경우 동일값 사용)
	int								tr_pctapnor;					// C상 공칭 Tap 위치 (상별 구분이 없는 경우 동일값 사용)
	double							tr_pctapstep;					// C상 Tap position 간 변화량 (상별 구분이 없는 경우 동일값 사용)
	int								tr_ii_connd;					// 제어노드로의 indirect index
	double							tr_pgr;					// 1차측 접지 저항
	double							tr_pgx;					// 1차측 접지 리액턴스
	int								tr_ii_mtr;
	int								tr_si_mtr;
	double							tr_sgr;					// 2차측 접지 저항
	double							tr_sgx;					// 2차측 접지 리액턴스
	int								tr_ii_ss;					// TR에서 SS로의 Indirect index
	int								tr_si_ss;					// TR에서 SS로의 Sibling index
	int								tr_loctap;					// TR에서 SS로의 Sibling index

	int								TR_LDCTYPE;
	double							TR_DVMMXV;
	double							TR_DVMMNV;



	int								tr_SI_FND;					// 선로 지공/가공 구분
	int								tr_II_FND;					// 선로 지공/가공 구분
	int								tr_SI_TND;					// 선로 지공/가공 구분
	int								tr_II_TND;					// 선로 지공/가공 구분
	int								tr_gbr;

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} TR_STA;
#define								TR_STA_SIZE						sizeof( TR_STA )


// Table : TR_DYN_UIN, Type : Online
typedef struct _TR_DYN_UIN
{
	int								tr_avr;					// 전압 제어 Mode (1: AVR, 0: Tap 제어)
	double							tr_patbsvl;					// 제어 모선의 A상 전압 목표치
	double							tr_padev;					// 제어 모선 A상의 전압 목표치 편차
	double							tr_pbtbsvl;					// 제어 모선의 B상 전압 목표치
	double							tr_pbdev;					// 제어 모선 B상의 전압 목표치 편차
	double							tr_pctbsvl;					// 제어 모선의 C상 전압 목표치
	double							tr_pcdev;					// 제어 모선 C상의 전압 목표치 편차
	int								tr_vvoflag;					// VVO 제어 참여 여부(0: 불참, 1: 참여)
	int								tr_con3p;					// 3상 일괄제어 여부(0: 상별제어, 1: 3상 일괄제어)
	int								tr_ii_pfvm;					// Power Factor Monitoring group으로의 indirect index
	double							tr_pacompr;					// A상 compensator R (부하 중심점까지의 등가 R값)
	double							tr_pacompx;					// A상 compensator X (부하 중심점까지의 등가 X값)
	double							tr_pbcompr;					// B상 compensator R (부하 중심점까지의 등가 R값)
	double							tr_pbcompx;					// B상 compensator X (부하 중심점까지의 등가 X값)
	double							tr_pccompr;					// C상 compensator R (부하 중심점까지의 등가 R값)
	double							tr_pccompx;					// C상 compensator X (부하 중심점까지의 등가 X값)
	int								tr_3conmeap;					// 3상 제어 TR의 LDC 기준 전압(1=A상, 2=B상, 3=C상)
	double							tr_patapcost;					// -
	double							tr_pacontapmx;					// -
	double							tr_pbtapcost;					// -
	double							tr_pbcontapmx;					// -
	double							tr_pctapcost;					// -
	double							tr_pccontapmx;					// -
	int								tr_pacontapmxday;					// A상 하루 최대 동작횟수
	int								tr_patapopday;					// A상 하루 동작횟수
	int								tr_pbcontapmxday;					// B상 하루 최대 동작횟수
	int								tr_pbtapopday;					// B상 하루 동작횟수
	int								tr_pccontapmxday;					// C상 하루 최대 동작횟수
	int								tr_pctapopday;					// C상 하루 동작횟수

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} TR_DYN_UIN;
#define								TR_DYN_UIN_SIZE						sizeof( TR_DYN_UIN )


// Table : VVM_STA, Type : Online
typedef struct _VVM_STA
{
	wchar_t							vvm_nm[64];					// Voltage violation monitoring group의 명칭

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} VVM_STA;
#define								VVM_STA_SIZE						sizeof( VVM_STA )


// Table : VVM_DYN_UIN, Type : Online
typedef struct _VVM_DYN_UIN
{
	int								vvm_morflag;					// VVM group의 감시 활성 여부(1: 감시, 0: 감시안함)
	double							vvm_lmhi;					// VVM group의 전압 high limit
	double							vvm_lmlo;					// VVM group의 전압 low limit
	double							vvm_tolhi;					// VVM 감시를 위한 tolerance 상한
	double							vvm_tollo;					// VVM 감시를 위한 tolerance 하한
	double							vvm_dband;					// VVM 감시를 위한 dead band

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} VVM_DYN_UIN;
#define								VVM_DYN_UIN_SIZE						sizeof( VVM_DYN_UIN )


// Table : VVOOPT_DYN_UIN, Type : Online
typedef struct _VVOOPT_DYN_UIN
{
	double							vvoopt_tolmx;					// 손실최소화 tolerance
	double							vvoopt_vemerstep;					// 전압 Emergency Step Size
	double							vvoopt_olemerstep;					// Overloading Emergency Step Size
	double							vvoopt_pfemerstep;					// Power factor Emergency Step Size
	int								vvoopt_emerlevelmx;					// 최대 emergency level
	int								vvoopt_elitermx;					// emergency level determination 최대 반복 연산 횟수
	int								vvoopt_scmitermx;					// SCM 최대 반복 연산 횟수
	int								vvoopt_lmitermx;					// LM 최대 반복 연산 횟수
	double							vvoopt_pfmslm;					// VVC 조류계산 최대 mismatch
	int								vvoopt_pfitermx;					// VVC 조류계산 최대 반복 연산 횟수
	int								vvoopt_scm;					// SCM 실행 여부(1: 실행, 0: 실행 안함)
	int								vvoopt_lm;					// LM 실행 여부(1: 실행, 0: 실행 안함)
	double							vvoopt_objmn;					// ELD와 SCM에서 사용되는 수렴 판단 기준, 선형 최적해의 목적함수 값의 절대값이 이 값 이하면 수렴으로 판단
	double							vvoopt_minloss;					// 손실최소화 결과 손실 감소량(kW)이 이 값 이상인 경우에만 지령값 출력
	int								vvoopt_scmobj;					// 스위칭 코스트 최소화의 목적함수(1: 비용, 2: 횟수)
	double							vvoopt_mvarmslm;					// 민감도 계산 시 최대 무효전력 수렴범위
	int								vvoopt_cvr;					// CVR실행여부(1: 실행, 0: 실행 안함)

	int								var_reser1;						// 예약 변수1
	int								var_reser2;						// 예약 변수2
	int								var_reser3;						// 예약 변수3
} VVOOPT_DYN_UIN;
#define								VVOOPT_DYN_UIN_SIZE						sizeof( VVOOPT_DYN_UIN )

//////////////김종민 추가 
typedef	struct	_TERMINAL
{
	wchar_t	terminal_id[64];
	wchar_t	terminal_ceqfk[64];
	wchar_t	terminal_cnfk[64];
	int		terminal_ceqtype;
	wchar_t	terminal_original_eqcfk[64];
	wchar_t	terminal_change_eqcfk[64];
	wchar_t	terminal_nm[64];
	int		terminal_nametype;
} TERMINAL;
#define								TERMINAL_SIZE						sizeof( TERMINAL )

typedef	struct	_BI_VALUE
{
	wchar_t	BI_CEQ_MRFK[ 32 ];
	int		BI_MASTER_CODE_FK;
	int		BI_VALUE_DATE;
} BI_VALUE;
#define								BI_VALUE_SIZE						sizeof( BI_VALUE )


typedef	struct	_NAME_TYPE
{
	wchar_t	NAME_MRID[64];
	int		NAME_TYPE_DATE;
	wchar_t	NAME_NAME[128];
	wchar_t	ALIAS_NAME[128];
} NAME_TYPE;
#define								NAME_TYPE_SIZE						sizeof( NAME_TYPE )


typedef struct _IDENTIFIEDOBJECT
{
	wchar_t		IDTER_MRID[64];
	int			IDTER_NAME_TYPE_FK;
	wchar_t		IDTER_NAME[64];
	wchar_t		IDTER_ALIAS_NAME[64];
	wchar_t		IDTER_TERMINAL_ID[64];
	wchar_t		IDTER_CEQ_MRFK[64];
	wchar_t		IDTER_CONNECTIVITYNODE_FK[64];

} IDENTIFIEDOBJECT;
#define								IDENTIFIEDOBJECT_SIZE						sizeof( IDENTIFIEDOBJECT )

typedef struct _conductingequipment
{
	wchar_t		MRID[64];
	int			CEQ_TYPE_FK;
	wchar_t		ORIGINAL_EQC_MRFK[64];
	wchar_t		CHANGE_EQC_MRFK[64];
	int			SEQ;

} conductingequipment;
#define								conductingequipment_SIZE						sizeof( conductingequipment )

typedef struct _LINESEGMENT_TYPE
{
	int			LINESEGMENT_TYPE_ID;
	int			OVERHEAD_CABLE;
	wchar_t		PHASE_LINE_TYPE[64];
	wchar_t		NEUTRAL_LINE_TYPE[64];
	double		POSITIVE_R;
	double		POSITIVE_X_1P;
	double		POSITIVE_X;
	double		ZERO_R;
	double		ZERO_X;
	double		THERMAL_LIMIT;
	double		R_KM;
	double		X_KM;

} LINESEGMENT_TYPE;
#define								LINESEGMENT_TYPE_SIZE						sizeof( LINESEGMENT_TYPE )

//Table : INNERPOLE_STA, Type : Online
typedef struct _LINESEGMENT_DETAIL
{
	int								LINESEGMENT_DETAIL_ID;
	wchar_t							LINE_MRID[64];
	int								HVL_SEQ;
	int								HVL_SYSID;
	int								HVL_DIRECTION;
	double							F_LONGITUDE;
	double							F_LATITUDE;
	double							B_LONGITUDE;
	double							B_LATITUDE;
} LINESEGMENT_DETAIL;
#define								LINESEGMENT_DETAIL_SIZE					sizeof( LINESEGMENT_DETAIL )

//Table : INNERPOLE_STA, Type : Online
typedef struct _GLINESEGMENT
{
	int								GLINESEGMENT_ID;
	wchar_t							GLINESEGMENT_MRID[64];
	int								GLINESEGMENT_TYPE;
	double							F_LONGITUDE[7100];
	double							F_LATITUDE[7100];
	double							F_x;
	double							F_y;
	int								INNERPOLE_II_LNSEC;
	double							LENGTH;
} GLINESEGMENT;
#define								GLINESEGMENT_SIZE					sizeof( GLINESEGMENT )

typedef struct _LINESYSID
{
	wchar_t							LINESYSID_ID[64];
	wchar_t							LINESYSID_COMNO[64];
	wchar_t							LINESYSID_POLENO[64];
	int								LINESYSID_CUSTOMER;
} LINESYSID;
#define								LINESYSID_SIZE					sizeof( LINESYSID )

//20210126
typedef struct _HVCUS_STA
{
	int								HVCUS_STA_ID;
	wchar_t							HVCUS_NM[64];
	wchar_t							HVCUS_CEQID[64];
	int								HVCUS_II_GND;
	int								HVCUS_SI_GND;
	double							HVCUS_CON_KVA;
	double							HVCUS_ITR_KVA;
	double							HVCUS_ITR_PZ;
	int								HVCUS_II_PRDE;
	int								HVCUS_II_IJ;

} HVCUS_STA;
#define								HVCUS_STA_SIZE					sizeof( HVCUS_STA )


////////////////
typedef struct _PRDE_STA
{
	wchar_t							PRDE_STA_NM[64];

	int								PRDE_OCRF_II_TCCSET	  ;
	int								PRDE_OCRD_II_TCCSET	  ;
	double							PRDE_OCR_Pickup_C	  ;
	double							PRDE_OCR_IIC		  ;
	int								PRDE_OCGRF_II_TCCSET  ;
	int								PRDE_OCGRD_II_TCCSET  ;
	double							PRDE_OCGR_Pickup_C	  ;
	double							PRDE_OCGR_IIC		  ;
	int								PRDE_TYPE			  ;
	int								PRDE_SET_GTYPE		  ;
	int								PRDE_OCR_NOF		  ;
	int								PRDE_OCR_NOD		  ;
	int								PRDE_OCGR_NOF		  ;
	int								PRDE_OCGR_NOD		  ;
	double							PRDE_OCRF_TMS		  ;
	double							PRDE_OCRF_TAS		  ;
	double							PRDE_OCRF_MRT		  ;
	double							PRDE_OCRD_TMS		  ;
	double							PRDE_OCRD_TAS		  ;
	double							PRDE_OCRD_MRT		  ;
	double							PRDE_OCGRF_TMS		  ;
	double							PRDE_OCGRF_TAS		  ;
	double							PRDE_OCGRF_MRT		  ;
	double							PRDE_OCGRD_TMS		  ;
	double							PRDE_OCGRD_TAS		  ;
	double							PRDE_OCGRD_MRT		  ;
	int								PRDE_OCR_CTR		  ;
	int								PRDE_OCGR_CTR		  ;
	double							PRDE_MX_LD_C_PHA	  ;

} PRDE_STA;
#define								PRDE_STA_SIZE					sizeof( PRDE_STA )
//20220530
typedef struct _HDOF_STA
{
	wchar_t							HDOF_NM[64];
	int								HDOF_CODE;
	int								HDOF_HI_CENTER;
	int								CENTER_HI_BOF;

} HDOF_STA;
#define								HDOF_STA_SIZE					sizeof( HDOF_STA )



//////////////////////////////////////////////////////////////////////////
#endif




