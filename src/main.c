
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#ifdef __CYGWIN__
#include <windows.h>
#endif

#include "main.h"
#include "conf.h"
#include "log.h"
#include "kad.h"
#include "utils.h"
#include "unix.h"
#include "net.h"
#include "values.h"
#include "results.h"
#include "peerfile.h"
#ifdef __CYGWIN__
#include "windows.h"
#endif

#ifdef LPD
#include "ext-lpd.h"
#endif
#ifdef AUTH
#include "ext-auth.h"
#endif
#ifdef DNS
#include "ext-dns.h"
#endif
#ifdef WEB
#include "ext-web.h"
#endif
#ifdef NSS
#include "ext-nss.h"
#endif
#ifdef CMD
#include "ext-cmd.h"
#endif
#ifdef FWD
#include "ext-fwd.h"
#endif


int main_start( void ) {
	/* Setup port-forwarding */
#ifdef FWD
	fwd_setup();
#endif

	/* Setup the Kademlia DHT */
	kad_setup();

	/* Setup handler to announce values */
	values_setup();

	/* Setup handler to expire results */
	results_setup();

	/* Setup import of peerfile  */
	peerfile_setup();

	/* Setup extensions */
#ifdef LPD
	lpd_setup();
#endif
#ifdef AUTH
	auth_setup();
#endif
#ifdef DNS
	dns_setup();
#endif
#ifdef WEB
	web_setup();
#endif
#ifdef NSS
	nss_setup();
#endif
#ifdef CMD
	cmd_setup();
#endif

	/* Loop over all sockets and FDs */
	net_loop();

	/* Export peers if a file is provided */
	peerfile_export();

#ifdef LPD
	lpd_free();
#endif

	conf_free();

	return 0;
}

#ifdef __CYGWIN__
int main( int argc, char **argv ) {

	conf_init();
	conf_load_args( argc, argv );

	if( gconf->is_daemon ) {
		gconf->use_syslog = 1;

		/* Fork before any threads are started */
		unix_fork();

		char path[MAX_PATH], *p;
		/* Change to C:\ directory or disk equivalent */
		if( GetModuleFileNameA( NULL, path, sizeof(path) ) && (p = strchr( path, '\\' )) ) {
			*(p+1) = 0;
			SetCurrentDirectoryA( path );
		}

		/* Close pipes */
		fclose( stderr );
		fclose( stdout );
		fclose( stdin );
	} else {
		conf_info();
	}

	/* Catch signals */
	windows_signals();

	/* Write pid file */
	if( gconf->pidfile ) {
		unix_write_pidfile( GetCurrentProcessId(), gconf->pidfile );
	}

	/* Drop privileges */
	unix_dropuid0();

	return main_start();
}
#else
int main( int argc, char **argv ) {

	conf_init();
	conf_load_args( argc, argv );

	if( gconf->is_daemon ) {
		gconf->use_syslog = 1;

		/* Fork before any threads are started */
		unix_fork();

		if( chdir( "/" ) != 0 ) {
			log_err( "UNX: Changing working directory to / failed: %s", strerror( errno ) );
		}

		/* Close pipes */
		fclose( stderr );
		fclose( stdout );
		fclose( stdin );
	} else {
		conf_info();
	}

	/* Catch signals */
	unix_signals();

	/* Write pid file */
	if( gconf->pidfile ) {
		unix_write_pidfile( getpid(), gconf->pidfile );
	}

	/* Drop privileges */
	unix_dropuid0();

	return main_start();
}
#endif
