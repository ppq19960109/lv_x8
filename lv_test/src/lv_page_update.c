

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_test_main.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *  STATIC VARIABLES
 **********************/

void lv_page_update_create(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);

    lv_obj_t *img = lv_img_create(page);
    lv_obj_align(img, LV_ALIGN_TOP_MID, 0, 40);
    lv_img_set_src(img, themesImagesPath "icon_logo.png");


}
