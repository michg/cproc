#include <stdint.h>
#include <string.h>
#include "util.h"
#include "cc.h"

const struct target *targ;

static const struct target alltargs[] = {
	{
		.name = "x86_64-sysv",
		.typewchar = &typeint,
		.typevalist = &(struct type){
			.kind = TYPEARRAY,
			.align = 8, .size = 24,
			.u.array = {1}, .base = &(struct type){
				.kind = TYPESTRUCT,
				.align = 8, .size = 24,
			},
		},
		.signedchar = 1,
		.regsize = 8,
	},
	{
		.name = "aarch64",
		.typevalist = &(struct type){
			.kind = TYPESTRUCT,
			.align = 8, .size = 32,
			.u.structunion.tag = "va_list",
		},
		.typewchar = &typeuint,
		.regsize = 8,
	},
	{
		.name = "riscv64",
		.typevalist = &(struct type){
			.kind = TYPEPOINTER, .prop = PROPSCALAR,
			.align = 8, .size = 8,
			.base = &typevoid,
		},
		.typewchar = &typeint,
		.regsize = 8,
	},
	{
		.name = "riscv32",
		.typevalist = &(struct type){
			.kind = TYPEPOINTER, .prop = PROPSCALAR,
			.align = 4, .size = 4,
			.base = &typevoid,
		},
		.typewchar = &typeint,
		.regsize = 4,
	}, 
};

void
targinit(const char *name)
{
	size_t i;

	if (!name) {
		/* TODO: provide a way to set this default */
		targ = &alltargs[0];
	}
	for (i = 0; i < LEN(alltargs) && !targ; ++i) {
		if (strcmp(alltargs[i].name, name) == 0)
			targ = &alltargs[i];
	}
	if (!targ)
		fatal("unknown target '%s'", name);
	typechar.u.basic.issigned = targ->signedchar;
	typeadjvalist = typeadjust(targ->typevalist);
	ptrsize = targ->regsize;
	if(ptrsize == 4) {
	    typelong = typelong4;
	    typeulong = typeulong4;    
	} else {
	    typelong = typelong8;
	    typeulong = typeulong8;  
	}
}
