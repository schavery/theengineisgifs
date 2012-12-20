<?php
/**
 * @file cron.php
 * performs some cron tasks, in conjunction with cron.bat
 *
 * win command
 * schtasks /create /tn "gif-task" /tr "c:\www\theenginisgifs\cron.bat" /sc daily /st 06:00
 * Steve Avery steve@theengineisred.com August 2012
 */
// TODOS:
// - get into laptop and refresh page
// - send tweet of latest
include('../inc/common.inc');

dl_new_imgs();
set_next();

?>