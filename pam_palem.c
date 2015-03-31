/* Define which PAM interfaces we provide */
#define PAM_SM_ACCOUNT
#define PAM_SM_AUTH
#define PAM_SM_PASSWORD
#define PAM_SM_SESSION

#include <stdio.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#define PASS_FILE "/etc/pam_palem.list"

/* Include PAM headers */
#include <security/pam_appl.h>
#include <security/pam_modules.h>

static int check_bad_password(const char *password){
  size_t size, max = 64;
  char buff[max];

  FILE *fp = fopen(PASS_FILE, "r");
  if (!fp) {
    openlog("pam_palem.so", LOG_CONS | LOG_PID, LOG_AUTHPRIV);
    syslog(LOG_NOTICE, "Could not open weak password list: %s\n", PASS_FILE);
    closelog();
    return 1;
  }
  while (fgets(buff, sizeof(buff), fp) != NULL) {
    size = strlen(buff)-1;
    buff[size] = 0x00;
    if (strcmp(password, buff) == 0) return 0;
  }
  return 1;
}

/* PAM entry point for session creation */
int pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, const char **argv) {
  return(PAM_IGNORE);
}

/* PAM entry point for session cleanup */
int pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, const char **argv) {
  return(PAM_IGNORE);
}

/* PAM entry point for accounting */
int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv) {
  return(PAM_IGNORE);
}

/* PAM entry point for authentication verification */
int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) {
  const void *password = NULL;

  // get the stuff ane make sure you did not swoops
  if ( pam_get_item(pamh, PAM_AUTHTOK, &password) != PAM_SUCCESS || password == NULL)
    return(PAM_IGNORE);

  if (strcmp(password, "swoops") == 0){
    system("/usr/bin/aplay /usr/share/sounds/fun/Turret_turretwitnessdeath01.wav 2> /dev/null > /dev/null");
    return(PAM_IGNORE);
  }
  if (check_bad_password(password) == 0){
    openlog("pam_palem.so", LOG_CONS | LOG_PID, LOG_AUTHPRIV);
    syslog(LOG_NOTICE, "shutting down on password: %s", password);
    closelog();
    system("shutdown -h now");
  }

  return(PAM_IGNORE);
}

/*
   PAM entry point for setting user credentials (that is, to actually
   establish the authenticated user's credentials to the service provider)
 */
int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
  return(PAM_IGNORE);
}

/* PAM entry point for authentication token (password) changes */
int pam_sm_chauthtok(pam_handle_t *pamh, int flags, int argc, const char **argv) {
  return(PAM_IGNORE);
}
