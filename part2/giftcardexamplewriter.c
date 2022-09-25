/*
 * Gift Card Example Writer
 * Original Author: Shoddycorp's Cut-Rate Contracting
 * Maintainer: ???
 * Comments added by Justin Cappos (JAC)
 * Modification Date: 8 July 2020
 */

#include <stdio.h>
#include "giftcard.h"

/* JAC: Why is this included? */
#include <time.h>

struct this_gift_card examplegc;
struct gift_card_data examplegcd;
struct gift_card_record_data examplegcrd;
struct gift_card_amount_change examplegcac;
struct gift_card_program loop_hang;

// Break it up so that we don't have long functions!  Good programming style!
//  (JAC: This is so wrong.  Global variable use / initialization is a 
//  terrible thing to do.)

//first crash, you set num_bytes from 116 to -1 // this causes a crash b/c the reader is unable to read from a negative position 
void setupgc() {
	examplegc.num_bytes = 401; // HANG CASE: Step 1: raise this to at least 401 bytes
	examplegc.gift_card_data = (void *) &examplegcd;
	examplegcd.merchant_id = "GiftCardz.com                   ";
	examplegcd.customer_id = "DuaneGreenes Store 1451         ";
	examplegcd.number_of_gift_card_records = 1;

	/* JAC: Something seems fishy... */
	examplegcd.gift_card_record_data = malloc(examplegcd.number_of_gift_card_records);
	/* JAC: here too! */
	examplegcd.gift_card_record_data[0] = (void*) &examplegcrd;
	examplegcrd.record_size_in_bytes = 44;
	examplegcrd.type_of_record = 3; // HANG CASE: Step 2: change type of record to 3
	examplegcrd.actual_record = (void*)&examplegcd;
	//possible 2nd crash case is an integer overflow 
	//something like 123456789 for amount_added?
	examplegcac.amount_added = 2000;
	examplegcac.actual_signature = "[ insert crypto signature here ]";
	
	loop_hang.message = malloc(32); // 32 bytes allocated to message
	loop_hang.program = malloc(256); // 256 bytes allocated to double header 
	loop_hang.program[0] = 9; // go to position 9 within Animate function in the giftcardreader
	loop_hang.program[1] = -3; // forces the pc in arg1 to position back in the giftcardreader

}


// moved into separate files to avoid erroenous style checker error...
/* JAC: opening and writing a fixed filename using a global is terrible style.
 *     the style checker was right!  This code is terrible   
 */
void writegc() {
	FILE *fd1;
	// JAC: Why don't any of these check for error return codes?!?
	fd1 = fopen("hang_test.gft","w");
	fwrite(&examplegc.num_bytes,4,1,fd1);
	fwrite(examplegcd.merchant_id,32,1,fd1);
	fwrite(examplegcd.customer_id,32,1,fd1);
	fwrite(&examplegcd.number_of_gift_card_records,4,1,fd1);
	fwrite(&examplegcrd.record_size_in_bytes,4,1,fd1);
	fwrite(&examplegcrd.type_of_record,4,1,fd1);
	fwrite(&examplegcac.amount_added,4,1,fd1);
	fwrite(examplegcac.actual_signature,32,1,fd1);
	fwrite(loop_hang.message, 32, 1, fd1); // writing the allocated bytes
	fwrite(loop_hang.program, 256, 1, fd1); // writing the allocated bytes
	fclose(fd1);
}

/* JAC: No args and return -1 for no reason!?! */
int main(void) {

	setupgc();
	writegc();
	return -1; 
}
