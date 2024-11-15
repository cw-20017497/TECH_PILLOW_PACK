#ifndef __CONFIG_H__
#define __CONFIG_H__


/* TEST
 *  -. 압축기 구속시간 초기화 기능 활성화
 *  -. 더미 탈빙 취소 기능 활성화
 */
#define  CONFIG_TEST                0  // 냉동, Skip 압축기, 더미탈빙 기능 활성화
#define  CONFIG_MMI                 0  // MMI 연동 모드
#define  CONFIG_TEST_LED            0  // LED LIGHTING TEST ( 주간/야간 모드 )
#define  CONFIG_TEST_SAVING         0  // 조도 테스트 ( 조도 진입 시간 단축 )
#define  CONFIG_TEST_TEMP_READ      0  // 온도 센서 상시 읽기 모드 ( 테스트 모드 )
#define  CONFIG_TEST_FILTER         0  // 필터 내구성 시험 ( 테스트 모드 )


#define CONFIG_TEST_FLOW_METER      1

/* 에러 감지 기능 설정/해제 */
#define  CONFIG_ERR_ALL 1           /* 0: 해제, 1: 설정 */

/* 통신 송수신 모니터링 기능 설정/해제 */
#define  DEBUG_COMM     1           /* 0: 해제, 1:설정 */

/* 버전 */
#define VERSION         1

#endif /* __CONFIG_H__ */
