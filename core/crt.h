//   Copyright 2013 by Pavel Chistyakov
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

// common static size collection
template<typename T, int maxcount = 128>
struct sdat
{
	T						data[maxcount];
	int						count;
	T*						add() { return &data[count++]; }
	void					add(T e) { *(add()) = e; }
	T*						addv() { for(int i = 0; i<count; i++) if(!data[i].isvalid()) return &data[i]; return add(); }
	void					clear() { count = 0; }
	int						find(const T e) { int i = 0; for(; i<count; i++) if(data[i]==e) return i; return -1; }
};
struct rect
{
	int						x1, y1;
	int						x2, y2;
	//
	inline bool				operator!=(const rect& rc) const { return rc.x1 != x1 || rc.x2 != x2 || rc.y1 != y1 || rc.y2 != y2; }
	inline bool				operator==(const rect& rc) const { return rc.x1 == x1 && rc.x2 == x2 && rc.y1 == y1 && rc.y2 == y2; }
	void					clear() { x1 = x2 = y1 = y2 = 0; }
	inline int				height() const { return y2 - y1; }
	bool					intersect(const rect& rc) const;
	bool					isvalid() { return x1 != 0 || x2 != 0 || y1 != 0 || y2 != 0; }
	void					offset(int x, int y) { x1 += x; x2 -= x; y1 += y; y2 -= y; }
	void					set(int px1, int py1, int px2, int py2) { x1 = px1; x2 = px2; y1 = py1; y2 = py2; }
	inline int				width() const { return x2 - x1; }
};
struct point
{
	short int				x;
	short int				y;
	inline bool				operator!=(const point pt) const { return pt.x != x || pt.y != y; }
	inline bool				operator==(const point pt) const { return pt.x == x && pt.y == y; }
	bool					in(const rect& rc) const { return x >= rc.x1 && x <= rc.x2 && y >= rc.y1 && y <= rc.y2; }
	bool					in(const point p1, const point p2, const point p3) const;
};
namespace io
{
	struct stream
	{
		enum relocations { Begin, End, Current };
		stream&				operator<<(const char* t);
		stream&				operator<<(const int n);
		unsigned char		get();
		unsigned short		getLE16();
		unsigned			getLE32();
		unsigned			gsize();
		virtual int			read(void* result, int count) = 0;
		virtual int			seek(int count, relocations rel) = 0;
		void				skip(int count) { seek(count, Current); }
		virtual int			write(const void* result, int count) = 0;
		int					write(int value) { return write(&value, sizeof(value)); }
		int					writes(short int value) { return write(&value, sizeof(value)); }
	};
	struct file : public stream
	{
		enum access { Read, Write, ReadWrite };
		file();
		file(const char* url, access right = Read);
		~file();
		void				close();
		bool				create(const char* url, access right);
		static void			dir(char* temp);
		static bool			exist(const char* url);
		bool				isvalid() const;
		static const char*	find(const char* mask);
		static void*		load(const char* url, int* size = 0, int additional = 0); // can release by delete operator
		virtual int			read(void* result, int count) override;
		static void			remove(const char* url);
		int					seek(int count, relocations rel) override;
		int					write(const void* result, int count) override;
	private:
		void*				handle;
	};
}
extern char					chupper(char c); // to upper reg
extern char					chlower(char c); // to lower reg
const char*					gstr(int id); // get name of string token (export)
extern bool					ischa(int c); // is alphabetical character?
extern int					locale; // current locale (defined external)
void						rmcpy(void* p1, const void* p2, int count);
void						rmmov(void* p1, const void* p2, int count);
void						rmset(void* p1, int value, int count);
int							rmcmp(const void* p1, const void* p2, int count);
void*						rmresize(void* ptr, unsigned size);
unsigned					rmsize(void* ptr);
const char*					szchr(const char* p1, char ch);
void						szcpy(char* p1, const char* p2);
void						szcpy(char* p1, const char* p2, int max_count);
const char*					szcmd();
int							szcmp(const char* p1, const char* p2);
int							szcmp(const char* p1, const char* p2, int count);
int							szcmpi(const char* p1, const char* p2);
int							szcmpi(const char* p1, const char* p2, int count);
const char*					szdup(const char *text, int size);
char*						szend(char* text);
inline const char*			szend(const char* text) { while(*text) text++; return text; }
extern int					szerror(char* result, int count, int id);
const char*					szext(const char* path);
const char*					szline(char* result, const char* p);
const char*					sznameof(const char* text);
void*						xfind(const void *key, const void *base, int count, int size, int(*compar)(const void* p1, const void* p2, void* param), void* param = 0);
const char*					szfname(const char* text); // get file name from string (no fail)
const char*					szfpath(char* path, const char* fullpath);
const char*					szmkpath(char* result, const char* fullpath, const char* filename, const char* ext = 0);
char*						sznum(char* p1, int num, int precision = 0, const char* empthy = 0, int radix = 10);
const char*					sznumber(int value, int percision = 0);
const char*					szpercent(int percent);
char*						szprint(char* dst, const char* src, ...);
inline const char*			szskipsp(const char* p) { while(*p==' ' || *p=='\t') p++; return p; }
const char*					szskipcr(const char* p);
const char*					szt(const char* en, const char* ru=0);
int							sz2num(const char* p1, const char** pp1 = 0);
void						swap(unsigned short& u);
void						swap(unsigned& u);
extern int					timeticks(); // get random seed
void						xsrand();
void						xsrand(unsigned seed);
int							xrand();
bool						xrun(const char* url, char* command_line, const char* directory, bool handle_output = false, bool hide_window = false);
void						xsort(void* base, int count, int size, int(*compare)(const void* p1, const void* p2, void* param), void* param = 0);
const char*					upper(const char* string);
inline void					szcat(char* p1, const char* p2) { szcpy((char*)szend(p1), p2); }
inline char*				szchr(char* text, char ch) { return (char*)szchr((const char*)text, ch); }
inline int					szlen(const char* text) { return szend((char*)text) - text; }
template<class T> inline T	imax(T a, T b) { return a > b ? a : b; }
template<class T> inline T	imin(T a, T b) { return a < b ? a : b; }
template<class T> inline T	iabs(T a) { return a > 0 ? a : -a; }
template<class T> inline void iswap(T& a, T& b) { T i = a; a = b; b = i; }