# C

- [Awesome C](https://github.com/oz123/awesome-c): references over everything

## Coding standard and complience

- [SEI CERT Coding Standards - CERT Secure Coding -
  Confluence](https://wiki.sei.cmu.edu/confluence/display/seccode/SEI+CERT+Coding+Standards)

## Error handling

- `<errno.h>` for POSIX error codes (numbers)
- [C POSIX library](https://en.wikipedia.org/wiki/C_POSIX_library)
- [Different ways to terminate a program in
  C](https://iq.opengenus.org/ways-to-terminate-a-program-in-c/)

```c
#include <errno.h>
#include <fcntl.h> // open function
#include <string.h> // strerror function

if ( open(argv[1], O_RDWR) == -1 ) {
	printf("%s: %s\n", argv[1], strerror(errno));
}
```

### fprintf vs perror

Presuming you have an `stderr`.

- `fprintf(stderr,...)`: only prints what you write
- `perror(...)`: prints to `stderr` with a built-in knowledge of error codes by
  `errno`.

## variables

- `static` keeps its value between invocations; scope: local (function or file)
- `extern` change scope to all files
- `const` constant, do not change its value (same address and value)[^variables]
- `register` save variable in CPU register instead of RAM for quick access;
  scope: local (function or file)
- `auto` default option, omitted: choose type automatically

Note: When `#define` is used, the preprocessor will go through the code and
replace every instance of the `#define` variable with the appropriate value.
Since the `#define` variable exists only in the file where it is created,
it is possible to have the same definition in another file with a completely
different value. This could lead to disastrous consequences.

[^variables]: in `const char`, if a new value is assigned, it will instead
  allocate a new address and leave the other one behind

## integer \& printfs

- [C data types](https://en.wikipedia.org/wiki/C_data_types)
- [Fixed-width integer types](https://en.wikipedia.org/wiki/C_data_types#inttypes.h)
- [Limits macros](https://en.cppreference.com/w/cpp/types/climits)

There is a number of pre-made integer lengths and manipulations.
You have to include the following libraries:

* `#include <inttypes.h>`: `(u)int(xx)_t` and printfs definitions
* `#include <stdint.h>`: `(u)int(xx)_t` definitions

This gives you the types

* `int16_t`: a signed integer with 16 bits of length[^integer-and-printfs-1]
* `uint64_t`: an unsigned integer with 64 bits of length

[^integer-and-printfs-1]: `int16`: a common abbreviation for `int16_t`, but this is not defined in any library

There are also helpful `define`s, such as `UINT16_MAX` (maximum value for an `uint16_t`).

When printing for debugging, one may use `%llu`, for `uint32_t` (e.g.). Note that this is hardware dependent and should never be used in a final software. For the correct way, one should use

* `%"PRIu32"`: `uint32_t`. Example: `uint32_t var = 3; printf("var = %" PRIu32 "\n", var);`
* `%"PRId16"`: `int16_t`

Do not use `%zu`, for this is know to be inaccurate.

## structs

The order of elements affect how much memory the struct takes. For example:

```c
typedef struct {
	uint8_t a; // 1 byte
	uint32_t b; // 4 bytes
} my_struct_t;
```

Even although the struct above has a total of 5 bytes, it will occupy 8, because
the compiler aligns every 4 bytes[^struct-1]. I.e:

```
12345678
a___bbbb
```

[^struct-1]: This may depend on the processor's architecture and compiler.

So, a struct `my_struct1_t` occupies less space than `my_struct2_t` (even if
they have the same fields).

```c
typedef struct {
	uint8_t a; // 1 byte
	uint8_t x; // 1 byte
	uint8_t y; // 1 byte
	uint8_t z; // 1 byte
	uint32_t b; // 4 bytes
} my_struct1_t;

typedef struct {
	uint8_t a; // 1 byte
	uint8_t x; // 1 byte
	uint8_t y; // 1 byte
	uint32_t b; // 4 bytes
	uint8_t z; // 1 byte
} my_struct2_t;
```

### braking down memory with unions and structs

```c
typedef union {
	uint16_t reg;
	struct {
		uint8_t a;
		uint8_t b;
	} ind;
} tests;

int main () {

	tests teste;
	teste.reg = 0xaa55; // = 43605
	// 0xaa = 170
	// 0x55 = 85

	printf("reg = %llu, a = %llu, b = %llu",teste.reg,teste.ind.a,teste.ind.b);

	return 0;
}
```

Yield `reg = 43605, a = 85, b = 170`.

### useful low-level memory manipulation

The following may be useful if you are working with standard or protocols:

```c
typedef union {
	struct {
		unsigned char byte1;
		unsigned char byte2;
		unsigned char byte3;
		unsigned char byte4;
	} bytes;
	unsigned int dword;
} HW_Register;

HW_Register reg;

reg.dword = 0x12345678;
reg.byte.byte3 = 4;
```

Even although this is not standard C, it behaves predictably in most compilers.

### union \& structs width

We can also declare widths within unions \& structs:

```c
union {
	uint32_t supported	: 1;
	uint32_t enabled	: 1;
	uint16_t nstreams	: 16;
};
```

## `const` and pointers

There are three different combinations:

1. pointed data is constant and cannot be change, yet the address contained in
   the pointer can change: `const int* pData = &data`
2. address contained in the pointer is constant and cannot be changed, yet the
   data pointed to can change: `int* const pData = &data`
3. both address and pointed data are constant and cannot be changed: `int data =
   0; const int* const pData = &data;` (pointer variable declaration is not
   required to be specified as `const`)

## types widths and format specifier

Link: [Main types](https://en.wikipedia.org/wiki/C_data_types#Main_types)

## shorten if statements

```c
for (int i = -2; i <= 2; ++i) {
	if (i) printf("Inside %i\n", i);
	else printf("Outside %i\n", i);
}
```

Prints:

```
Inside -2
Inside -1
Outside 0
Inside 1
Inside 2
`

Remember: 0 = false, the rest is true

## strlen and sizeof

strlen does not count `\0`. If there is no null termination, behavior is
erratic.

## Head \& tail confusion

There is no standard for this one, but most people use refer these terms as a
queue of people. Therefore:

- head: the consumer, first one to be removed, read side;
- tail: the producer, last one to enter the queue, write side.

To remember: at McDonald's, the head is in front of the cashier and the tail is
the end of the line. People leave at the head and enter at the tail.

This is equivalent to the NVMe's specification.

## Mod funcion

```c
int remainder = number % divisor;
```

Examples:

- `-3 % 2` = -1
- `12 % 4` = 0
- `54 % -7` = 5

## free (function)

For example:

```c
mystruct_t* mystruct = (mystruct*) calloc(100, sizeof(mystruct));
free(mystruct);
```

Every 100 `mystruct_t` are freed (there is no memory leakage).

## random number

For quick debugging, you may want to do the following:

```c
#include <time.h>

srandom(time(NULL)); // introduce the seed, which is the timestamp since 1970 in seconds
int ran = random(); // get a random value from seed
int a = ran % 10; // get a random value from ran, with maximum output of 9
```

Or, simply (`index` is the seed):

```c
uint f_randi(uint32_t index)
{
	index = (index << 13) ^ index;
	return ((index * (index * index * 15731 + 789221) + 1376312589) &
	0x7fffffff);
}
```

## memory allocation

When allocating, always prefer this `int* a = (int*) malloc(sizeof *a);` instead
of `int* a = (int*) malloc(sizeof(int*));`, because if the type changes the
first one will not have to change.

You may be intrested in gflags: this forces the memory to be 4 bytes spaced,
with each spacing being forbidden (program crashes).

## pointer tricks

It is faster to do:

```c
void *end;
for(end = &p[10]; p != end; p++)
	*p = 0;
```

Instead of this:

```c
for(int i = 0; i < 10; i++)
	p[i] = 0;
```

Because the first one does not require to do additional math.

## pointers as arguments to functions

It is always a good idea to test whether the pointer supplied to the function is
not null:

```c
void CalcArea(const double* const pPi, const double* const pRadius, double* const pArea)
{
	// check pointers before using!
	if (pPi && pRadius && pArea)
		*pArea = (*pPi) * (*pRadius) * (*pRadius);
}
```


## arrays vs pointers

Memory takes much more time than computing a value. Arrays are much better than structs. Use structs if that is the last resource.

Arrays are basically the same as pointers. If you need to add more elements, you
can double the size:

```c
if(array_length == array_allocated)
{
	array_allocated *= 2;
	array = realloc(array, (sizeof *array) * array_allocated);
}
array[array_lengh] = value;
array_length++;

// remove
array[i] = array[--array_lengh];

// backwards remove is faster then linked lists
for(i = array_length; i != 0;)
{
	i--;
	if(array[i] == the_one_we_want_to_remove)
		break;
	last = save;
	save = array[i];
```

Be careful to not allocate twice:

```c
typedef struct {
	int length;
	uint8_t *data;
} my_array_t;

my_array_t *array;
array = malloc(sizeof *array);
array->length = length;
array->data = malloc((sizeof *array->data) * array->length);

for(i = 0; i < length; i++)
	array->data[i] = 0;
```

Use the following instead:

```c
typedef struct {
	int length;
	uint8_t data[1];
} my_array_t;

my_array_t *array;
array->data = malloc((sizeof *array) + sizeof(int) * (length - 1));
array->length = length;
```

## square root algorithm

```c
float Q_rsqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = * (long * ) &y; // bit-by-bit copy float->int
	i = 0x5f3759df - (i >> 1); // dividing and improving the approximation
	y = * ( float * ) &i; // bit-by-bit copy int->float
	y = y * (threehalfs - (x2 * y * y)); // 1st Netwon iteration
	//y = y * (threehalfs - (x2 * y * y)); // 2st Netwon iteration
	return y;
}
```

### bit-by-bit copy among types

This is expressed in the line

```c
long i;
float y;
i = * ( long * ) &y;
```

Explanation follows:

- take the address of the input type (`float`)
- transforms the address of the input type to the output type (`long`): this is
  where the magic happens
- interprets the address as the output type (`long`)

## Command-line arguments

[Parsing command line arguments from
C](https://yakking.branchable.com/posts/parsing-command-line-arguments-from-c/)

```c
void main(int argc, char *argv[]);
```

- `argc` = argument count: the number of arguments, 1 based value
- `argv` = argument vector: a pointer to the arguments

`argv[0]` is always how the name of the compiled source. E.g. echo command,
`argv[0]  = "echo"`.

- `test oi tchau`: argc = 3
- `test "oi tchau"`: argc = 2

You can also employ the GNU `getopt` library as such:

```c
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

	int
main (int argc, char **argv)
{
	int aflag = 0;
	int bflag = 0;
	char *cvalue = NULL;
	int index;
	int c;

	opterr = 0;

	while ((c = getopt (argc, argv, "abc:")) != -1)
		switch (c)
		{
			case 'a':
				aflag = 1;
				break;
			case 'b':
				bflag = 1;
				break;
			case 'c':
				cvalue = optarg;
				break;
			case '?':
				if (optopt == 'c')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr,
							"Unknown option character `\\x%x'.\n",
							optopt);
				return 1;
			default:
				abort ();
		}

	printf ("aflag = %d, bflag = %d, cvalue = %s\n",
			aflag, bflag, cvalue);

	for (index = optind; index < argc; index++)
		printf ("Non-option argument %s\n", argv[index]);
	return 0;
}
```

Note that, inside the `getopt` function:

- `c` means it does not take a value
- `c:` means it does take a value
- `c::` means it does take a value, but its optional

```sh
% testopt
aflag = 0, bflag = 0, cvalue = (null)

% testopt -a -b
aflag = 1, bflag = 1, cvalue = (null)

% testopt -ab
aflag = 1, bflag = 1, cvalue = (null)

% testopt -c foo
aflag = 0, bflag = 0, cvalue = foo

% testopt -cfoo
aflag = 0, bflag = 0, cvalue = foo

% testopt arg1
aflag = 0, bflag = 0, cvalue = (null)
Non-option argument arg1

% testopt -a arg1
aflag = 1, bflag = 0, cvalue = (null)
Non-option argument arg1

% testopt -c foo arg1
aflag = 0, bflag = 0, cvalue = foo
Non-option argument arg1

% testopt -a -- -b
aflag = 1, bflag = 0, cvalue = (null)
Non-option argument -b

% testopt -a -
aflag = 1, bflag = 0, cvalue = (null)
Non-option argument -
```

## Macros

Avoid function like macros. But if you want to use these badly, consider:

```c
#define quad(x) (x*x)
int a = quad(2+3);
```

Will expand to:

```c
int a = (2+3*2+3);
```

## Macros with optional arguments

[Macros wtih a Variable Number of
Arguments](https://gcc.gnu.org/onlinedocs/gcc/Variadic-Macros.html)

Be careful with this: compilers may not support it (`gcc` does).

This type of macro is called "variadic macro".

```c
#define debug(format, ...) fprintf (stderr, format, __VA_ARGS__)
```

Here `...` is a variable argument. In the invocation of such a macro, it
represents the zero or more tokens until the closing parenthesis that ends the
invocation, including any commas. This set of tokens replaces the identifier
`__VA_ARGS__` in the macro body wherever it appears.

GCC has long supported variadic macros, and used a different syntax that allowed
you to give a name to the variable arguments just like any other argument.

```c
#define debug(format, args...) fprintf (stderr, format, args)
```

There is also the `##` operator, that removes the comma before it, if next
argument is not given.

```c
#define debug(format, ...) fprintf (stderr, format, ## __VA_ARGS__)
```

#define debug(format, args...) fprintf (stderr, format, args)

## mimicking C++ class

```c
typedef struct {
	float (*computeArea)(const ShapeClass *shape);
} ShapeClass;

float shape_computeArea(const ShapeClass *shape)
{
	return shape->computeArea(shape);
}

/* iheriting base class */
typedef struct {
	ShapeClass shape;
	float width, height;
} RectangleClass;

/* implementing function */
static float rectangle_computeArea(const ShapeClass *shape)
{
	const RectangleClass *rect = (const RectangleClass *) shape;
	return rect->width * rect->height;
}

/* constructor */
void rectangle_new(RectangleClass *rect)
{
	rect->width = rect->height = 0.f;
	rect->shape.computeArea = rectangle_computeArea;
}

/* another constructor */
void rectangle_new_with_lengths(RectangleClass *rect, float width, float height)
{
	rectangle_new(rect);
	rect->width = width;
	rect->height = height;
}

int main(void)
{
	RectangleClass r1;

	rectangle_new_with_lengths(&r1, 4.f, 5.f);
	printf("rectangle r1's area is %f units square\n", shape_computeArea(&r1));
	return 0;
}
```

## comments

Use

```c
#pragma warning "comentario"
```

This will print `comentario` at the compilation output.

## OOP

This is just an example of how a logger is implemented in OOP:

- Separate structures for object and interface (available methods)
- Interface struct may be reused
- Methods are assigned as a function, which must be declared independently

```c
typedef struct logger_ logger;
typedef struct log_interface_ log_interface;

// struct for object
struct logger_ {
	log_interface *interface;
	void *data;
};

// struct for methods (interface)
struct log_interface_ {
	int (*init)(logger *self, void *data);
	void (*info)(logger *self, char *str);
	void (*error)(logger *self, char *str);
	void (*close)(logger *self);
};

// functions declarations
int file_init(logger *self, void *data);
void file_info(logger *self, char *str);
void file_error(logger *self, char *str);
void file_close(logger *self);

// linking methods to functions
log_interface file_interface = {
	.init = file_init,
	.info = file_info,
	.error = file_error,
	.close = file_close
};

// struct for handling file access (not important)
typedef struct filelog_data_ {
	char *filename;
	FILE *fd;
} filelog_data;

// functions implementation
int file_init(logger *self, void *data) {

	self->interface = &file_interface;
	self->data = data;

	filelog_data *fld = data;
	fld->fd = fopen(fld->filename, "w");

	printf("File Logger Init: %s - %p\n", fld->filename, fld->fd);

	if (!fld->fd) {
		return errno;
	}

	return 0;
}

int filelog_create(logger *impl, char *filename) {
	filelog_data *data = malloc(sizeof(filelog_data));

	if (!data) {
		return errno;
	}

	data->filename = filename;

	return file_init(impl, data);
}

void file_info(logger *self, char *str) {
	filelog_data *fld = self->data;
	fprintf(fld->fd, "INFO :> %s\n", str);
}

void file_error(logger *self, char *str) {
	filelog_data *fld = self->data;
	fprintf(fld->fd, "ERROR:> %s\n", str);
}

void file_close(logger *self) {
	filelog_data *fld = self->data;
	fclose(fld->fd);
	free(fld);
}
```

## exec

The exec family of functions replaces the current running process with a new
process. It can be used to run a C program by using another C program. It comes
under the header file unistd.h.

Some variations are available and those can be understood letter-by-letter:

- `execv`: "exec vector", meaning that you pass in the arguments as a vector of strings
- `execl`: "exec list", pass the arguments as parameters
- `execXp`: search in `$PATH`, if unused, full path must be given
- `execXp?e`: pass different environment

```c++
char *args[] = {"/usr/bin/ls", "-l", (char*) NULL};
execv(args[0], args);
execl("/usr/bin/ls", "-l", (char*) NULL)
```

## typedef function signature

Note: function signature = return type + parameters types.

Typedef for a function signature is to make it easier to declare function pointers.

```c
void bar() {...}

typedef void (*foo)();
foo foobar; // compile equally as void (*foobar)();
foobar = &bar;

int main() {
	foobar();
}
```
## overwrite symbols

In the linker part there are strong and weak symbols. A strong symbol will
overwrite a weak symbol. Weak symbol will be declared as in the example below:

```c
void __attribute__((__weak__)) test() { ... }
```
