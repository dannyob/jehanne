#define NS2MS(ns) ((ns) / 1000000L)
#define S2MS(s)   ((s)  * 1000LL)

#define timems()	NS2MS(nsec())

typedef struct Ndbtuple Ndbtuple;

enum
{
	/* RR types; see: http://www.iana.org/assignments/dns-parameters */
	Ta=		1,
	Tns=		2,
	Tmd=		3,
	Tmf=		4,
	Tcname=		5,
	Tsoa=		6,
	Tmb=		7,
	Tmg=		8,
	Tmr=		9,
	Tnull=		10,
	Twks=		11,
	Tptr=		12,
	Thinfo=		13,
	Tminfo=		14,
	Tmx=		15,
	Ttxt=		16,
	Trp=		17,
	Tafsdb=		18,
	Tx25=		19,
	Tisdn=		20,
	Trt=		21,
	Tnsap=		22,
	Tnsapptr=	23,
	Tsig=		24,
	Tkey=		25,
	Tpx=		26,
	Tgpos=		27,
	Taaaa=		28,
	Tloc=		29,
	Tnxt=		30,
	Teid=		31,
	Tnimloc=	32,
	Tsrv=		33,
	Tatma=		34,
	Tnaptr=		35,
	Tkx=		36,
	Tcert=		37,
	Ta6=		38,
	Tdname=		39,
	Tsink=		40,
	Topt=		41,
	Tapl=		42,
	Tds=		43,
	Tsshfp=		44,
	Tipseckey=	45,
	Trrsig=		46,
	Tnsec=		47,
	Tdnskey=	48,

	Tspf=		99,
	Tuinfo=		100,
	Tuid=		101,
	Tgid=		102,
	Tunspec=	103,

	/* query types (all RR types are also queries) */
	Ttkey=	249,	/* transaction key */
	Ttsig=	250,	/* transaction signature */
	Tixfr=	251,	/* incremental zone transfer */
	Taxfr=	252,	/* zone transfer */
	Tmailb=	253,	/* { Tmb, Tmg, Tmr } */
	Tmaila= 254,	/* obsolete */
	Tall=	255,	/* all records */

	/* classes */
	Csym=	0,	/* internal symbols */
	Cin=	1,	/* internet */
	Ccs,		/* CSNET (obsolete) */
	Cch,		/* Chaos net */
	Chs,		/* Hesiod (?) */

	/* class queries (all class types are also queries) */
	Call=	255,	/* all classes */

	/* opcodes */
	Oquery=		0<<11,		/* normal query */
	Oinverse=	1<<11,		/* inverse query (retired) */
	Ostatus=	2<<11,		/* status request */
	Onotify=	4<<11,		/* notify slaves of updates */
	Oupdate=	5<<11,
	Omask=		0xf<<11,	/* mask for opcode */

	/* response codes */
	Rok=		0,
	Rformat=	1,	/* format error */
	Rserver=	2,	/* server failure (e.g. no answer from something) */
	Rname=		3,	/* bad name */
	Runimplimented=	4,	/* unimplemented */
	Rrefused=	5,	/* we don't like you */
	Ryxdomain=	6,	/* name exists when it should not */
	Ryxrrset=	7,	/* rr set exists when it should not */
	Rnxrrset=	8,	/* rr set that should exist does not */
	Rnotauth=	9,	/* not authoritative */
	Rnotzone=	10,	/* name not in zone */
	Rbadvers=	16,	/* bad opt version */
/*	Rbadsig=	16, */	/* also tsig signature failure */
	Rbadkey=	17,		/* key not recognized */
	Rbadtime=	18,		/* signature out of time window */
	Rbadmode=	19,		/* bad tkey mode */
	Rbadname=	20,		/* duplicate key name */
	Rbadalg=	21,		/* algorithm not supported */
	Rmask=		0x1f,	/* mask for response */
	Rtimeout=	1<<5,	/* timeout sending (for internal use only) */

	/* bits in flag word (other than opcode and response) */
	Fresp=		1<<15,	/* message is a response */
	Fauth=		1<<10,	/* true if an authoritative response */
	Ftrunc=		1<<9,	/* truncated message */
	Frecurse=	1<<8,	/* request recursion */
	Fcanrec=	1<<7,	/* server can recurse */

	Domlen=		256,	/* max domain name length (with NULL) */
	Labellen=	64,	/* max domain label length (with NULL) */
	Strlen=		256,	/* max string length (with NULL) */

	/* time to live values (in seconds) */
	Min=		60,
	Hour=		60*Min,		/* */
	Day=		24*Hour,	/* Ta, Tmx */
	Week=		7*Day,		/* Tsoa, Tns */
	Year=		52*Week,
	DEFTTL=		Day,

	/* reserved time (can't be timed out earlier) */
	Reserved=	5*Min,

	/* packet sizes */
	Maxudp=		512,	/* maximum bytes per udp message sent */
	Maxudpin=	2048,	/* maximum bytes per udp message rcv'd */

	/* length of domain name hash table */
	HTLEN= 		4*1024,

	Maxpath=	128,	/* size of mntpt */
	Maxlcks=	10,	/* max. query-type locks per domain name */

	RRmagic=	0xdeadbabe,
	DNmagic=	0xa110a110,

	/* parallelism: tune; was 32; allow lots */
	Maxactive=	250,

	/* tune; was 60*1000; keep it short */
	Maxreqtm=	8*1000,	/* max. ms to process a request */

	Notauthoritative = 0,
	Authoritative,
};

typedef struct Area	Area;
typedef struct Block	Block;
typedef struct Cert	Cert;
typedef struct DN	DN;
typedef struct DNSmsg	DNSmsg;
typedef struct Key	Key;
typedef struct Null	Null;
typedef struct RR	RR;
typedef struct Request	Request;
typedef struct SOA	SOA;
typedef struct Server	Server;
typedef struct Sig	Sig;
typedef struct Srv	Srv;
typedef struct Txt	Txt;

/*
 *  a structure to track a request and any slave process handling it
 */
struct Request
{
	int	isslave;	/* pid of slave */
	uint64_t	aborttime;	/* time in ms at which we give up */
	jmp_buf	mret;		/* where master jumps to after starting a slave */
	int	id;
	char	*from;		/* who asked us? */
	void	*aux;
};

/*
 *  a domain name
 */
struct DN
{
	DN	*next;		/* hash collision list */
	uint32_t	magic;
	char	*name;		/* owner */
	RR	*rr;		/* resource records off this name */
	uint32_t	referenced;	/* time last referenced */
	uint32_t	ordinal;
	uint16_t	class;		/* RR class */
	uint8_t	respcode;	/* response code */
	uint8_t	mark;		/* for mark and sweep */
};

/*
 *  security info
 */
struct Block
{
	int	dlen;
	uint8_t	*data;
};
struct Key
{
	int	flags;
	int	proto;
	int	alg;
	Block;
};
struct Cert
{
	int	type;
	int	tag;
	int	alg;
	Block;
};
struct Sig
{
	Cert;
	int	labels;
	uint32_t	ttl;
	uint32_t	exp;
	uint32_t	incep;
	DN	*signer;
};
struct Null
{
	Block;
};

/*
 *  text strings
 */
struct Txt
{
	Txt	*next;
	char	*p;
};

/*
 *  an unpacked resource record
 */
struct RR
{
	RR	*next;
	uint32_t	magic;
	DN	*owner;		/* domain that owns this resource record */
	uintptr	pc;		/* for tracking memory allocation */
	uint32_t	ttl;		/* time to live to be passed on */
	uint32_t	expire;		/* time this entry expires locally */
	uint32_t	marker;		/* used locally when scanning rrlists */
	uint16_t	type;		/* RR type */
	uint16_t	query;		/* query type is in response to */
	uint8_t	auth;		/* flag: authoritative */
	uint8_t	db;		/* flag: from database */
	uint8_t	cached;		/* flag: rr in cache */
	uint8_t	negative;	/* flag: this is a cached negative response */

	union {			/* discriminated by negative & type */
		DN	*negsoaowner;	/* soa for cached negative response */
		DN	*host;	/* hostname - soa, cname, mb, md, mf, mx, ns, srv */
		DN	*cpu;	/* cpu type - hinfo */
		DN	*mb;	/* mailbox - mg, minfo */
		DN	*ip;	/* ip address - a, aaaa */
		DN	*rp;	/* rp arg - rp */
		uintptr	arg0;	/* arg[01] are compared to find dups in dn.c */
	};
	union {			/* discriminated by negative & type */
		int	negrcode; /* response code for cached negative resp. */
		DN	*rmb;	/* responsible maibox - minfo, soa, rp */
		DN	*ptr;	/* pointer to domain name - ptr */
		DN	*os;	/* operating system - hinfo */
		uint32_t	pref;	/* preference value - mx */
		uint32_t	local;	/* ns served from local database - ns */
		uint16_t	port;	/* - srv */
		uintptr	arg1;	/* arg[01] are compared to find dups in dn.c */
	};
	union {			/* discriminated by type */
		SOA	*soa;	/* soa timers - soa */
		Srv	*srv;
		Key	*key;
		Cert	*cert;
		Sig	*sig;
		Null	*null;
		Txt	*txt;
	};
};

/*
 *  list of servers
 */
struct Server
{
	Server	*next;
	char	*name;
};

/*
 *  timers for a start-of-authority record.  all uint32_ts are in seconds.
 */
struct SOA
{
	uint32_t	serial;		/* zone serial # */
	uint32_t	refresh;	/* zone refresh interval */
	uint32_t	retry;		/* zone retry interval */
	uint32_t	expire;		/* time to expiration */
	uint32_t	minttl;		/* min. time to live for any entry */

	Server	*slaves;	/* slave servers */
};

/*
 * srv (service location) record (rfc2782):
 * _service._proto.name ttl class(IN) 'SRV' priority weight port target
 */
struct Srv
{
	uint16_t	pri;
	uint16_t	weight;
};

typedef struct Rrlist Rrlist;
struct Rrlist
{
	int	count;
	RR	*rrs;
};

/*
 *  domain messages
 */
struct DNSmsg
{
	uint16_t	id;
	int	flags;
	int	qdcount;	/* questions */
	RR 	*qd;
	int	ancount;	/* answers */
	RR	*an;
	int	nscount;	/* name servers */
	RR	*ns;
	int	arcount;	/* hints */
	RR	*ar;
};

/*
 *  definition of local area for dblookup
 */
struct Area
{
	Area	*next;

	int	len;		/* strlen(area->soarr->owner->name) */
	RR	*soarr;		/* soa defining this area */
	int	neednotify;
	int	needrefresh;
};

typedef struct Cfg Cfg;
struct Cfg {
	int	cachedb;
	int	resolver;
	int	justforw;	/* flag: pure resolver, just forward queries */
	int	serve;		/* flag: serve udp queries */
	int	inside;
	int	straddle;
};

/* (udp) query stats */
typedef struct {
	QLock;
	uint32_t	slavehiwat;	/* procs */
	uint32_t	qrecvd9p;	/* query counts */
	uint32_t	qrecvdudp;
	uint32_t	qsent;
	uint32_t	qrecvd9prpc;	/* packet count */
	uint32_t	alarms;
	/* reply times by count */
	uint32_t	under10ths[3*10+2];	/* under n*0.1 seconds, n is index */
	uint32_t	tmout;
	uint32_t	tmoutcname;
	uint32_t	tmoutv6;

	uint32_t	answinmem;	/* answers in memory */
	uint32_t	negans;		/* negative answers received */
	uint32_t	negserver;	/* neg ans with Rserver set */
	uint32_t	negbaddeleg;	/* neg ans with bad delegations */
	uint32_t	negbdnoans;	/* ⋯ and no answers */
	uint32_t	negnorname;	/* neg ans with no Rname set */
	uint32_t	negcached;	/* neg ans cached */
} Stats;

Stats stats;

enum
{
	Recurse,
	Dontrecurse,
	NOneg,
	OKneg,
};

extern Cfg	cfg;
extern char	*dbfile;
extern int	debug;
extern Area	*delegated;
extern char	*logfile;
extern int	maxage;		/* age of oldest entry in cache (secs) */
extern char	mntpt[];
extern int	needrefresh;
extern int	norecursion;
extern uint32_t	now;		/* time base */
extern int64_t	nowns;
extern Area	*owned;
extern int	sendnotifies;
extern uint32_t	target;
extern int	testing;	/* test cache whenever removing a DN */
extern char	*trace;
extern int	traceactivity;
extern char	*zonerefreshprogram;

#pragma	varargck	type	"R"	RR*
#pragma	varargck	type	"Q"	RR*


/* dn.c */
extern char	*rrtname[];
extern char	*rname[];
extern unsigned	nrname;
extern char	*opname[];
extern Lock	dnlock;

void	addserver(Server**, char*);
Server*	copyserverlist(Server*);
void	db2cache(int);
void	dnage(DN*);
void	dnageall(int);
void	dnagedb(void);
void	dnagenever(DN *);
void	dnauthdb(void);
void	dndump(char*);
void	dnget(void);
void	dninit(void);
DN*	dnlookup(char*, int, int);
DN*	idnlookup(char*, int, int);
void	dnptr(uint8_t*, uint8_t*, char*, int, int, int);
void	dnpurge(void);
void	dnput(void);
void	dnslog(char*, ...);
void	dnstats(char *file);
void*	emalloc(int);
char*	estrdup(char*);
void	freeanswers(DNSmsg *mp);
void	freeserverlist(Server*);
int	getactivity(Request*, int);
Area*	inmyarea(char*);
void	putactivity(int);
RR*	randomize(RR*);
RR*	rralloc(int);
void	rrattach(RR*, int);
int	rravfmt(Fmt*);
RR*	rrcat(RR**, RR*);
RR**	rrcopy(RR*, RR**);
int	rrfmt(Fmt*);
void	rrfree(RR*);
void	rrfreelist(RR*);
RR*	rrlookup(DN*, int, int);
char*	rrname(int, char*, int);
RR*	rrremneg(RR**);
RR*	rrremtype(RR**, int);
RR*	rrremowner(RR**, DN*);
RR*	rrremfilter(RR**, int (*)(RR*, void*), void*);
int	rrsupported(int);
int	rrtype(char*);
void	slave(Request*);
int	subsume(char*, char*);
int	tsame(int, int);
void	unique(RR*);
void	warning(char*, ...);

/* dnarea.c */
void	refresh_areas(Area*);
void	freearea(Area**);
void	addarea(DN *dp, RR *rp, Ndbtuple *t);

/* dblookup.c */
int	baddelegation(RR*, RR*, uint8_t*);
RR*	dbinaddr(DN*, int);
RR*	dblookup(char*, int, int, int, int);
RR*	dnsservers(int);
RR*	domainlist(int);
int	insideaddr(char *dom);
int	insidens(uint8_t *ip);
int	myaddr(char *addr);
int	opendatabase(void);
int	outsidensip(int, uint8_t *ip);

/* dns.c */
char*	walkup(char*);
RR*	getdnsservers(int);
void	logreply(int, uint8_t*, DNSmsg*);
void	logsend(int, int, uint8_t*, char*, char*, int);
void	procsetname(char *fmt, ...);

/* dnresolve.c */
RR*	dnresolve(char*, int, int, Request*, RR**, int, int, int, int*);
int	udpport(char *);
int	mkreq(DN *dp, int type, uint8_t *buf, int flags, uint16_t reqno);
int	seerootns(void);
void	initdnsmsg(DNSmsg *mp, RR *rp, int flags, uint16_t reqno);

/* dnserver.c */
void	dnserver(DNSmsg*, DNSmsg*, Request*, uint8_t *, int);
void	dnudpserver(char*);

/* dnnotify.c */
void	dnnotify(DNSmsg*, DNSmsg*, Request*);
void	notifyproc(void);

/* convDNS2M.c */
int	convDNS2M(DNSmsg*, uint8_t*, int);

/* convM2DNS.c */
char*	convM2DNS(uint8_t*, int, DNSmsg*, int*);

/* idn.c */
char*	utf2idn(char *, char *, int);
char*	idn2utf(char *, char *, int);

#pragma varargck argpos dnslog 1
