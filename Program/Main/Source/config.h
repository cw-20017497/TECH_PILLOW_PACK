#ifndef __CONFIG_H__
#define __CONFIG_H__


/* TEST
 *  -. ??축???구속??간 초기???기?????화
 *  -. ??????빙 취소 기?????화
 */
#define  CONFIG_TEST                0  // ??? Skip ??축??? ??미탈빙 기?????화
#define  CONFIG_MMI                 0  // SET: PC Debug Mode, CLEAR: Line Auto Test Mode
#define  CONFIG_TEST_LED            0  // LED LIGHTING TEST ( 주??/???? 모드 )
#define  CONFIG_TEST_SAVING         0  // 조도 ?????( 조도 ??????간 ???? )
#define  CONFIG_TEST_TEMP_READ      0  // ??도 ??서 ?????기 모드 ( ?????모드 )
#define  CONFIG_TEST_COLD_SENSOR    0  // SET: 1sec???? ?ü����? turn on, CLEAR: ???????? (40sec)


#define  CONFIG_ERR_ALL             0  /* SET: Err Detect, CLEAR: Err Undetect?? */
#define  DEBUG_COMM                 1  /* 0: ??제, 1:??정 */
#define  VERSION                    1

#endif /* __CONFIG_H__ */
