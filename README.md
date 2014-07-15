#Pindel2BAM 
Convert [Pindel](https://github.com/ding-lab/pindel2) output into BAM format.  

##Compiling
To convert from Pindel to BAM, first compile pindel2sam.cpp with the 
provided Make file. Pindel2BAM can compile with gcc version 4.4.3.

##Usage
Pindel2BAM takes four inputs in the following order:
* Pindel data directory
* desired output directory
* Pindel config file that was used to generate the Pindel data
* Genome reference index file (fa.fai) used to generate the Pindel data

Only the deletion and short insertion data are used (_D & _SI),
and all Pindel data files within the Pindel data directory provided
will be read, converted, and compiled initially into SAM files
(same file names as in the config file with the .sam file extension
appended). The SAM files are written into the desired output
directory and converted to BAM files. Finally, the BAM files are sorted
and indexed using the following samtools commands.

samtools view -bS convertedfilename.sam > bamfilename.bam  
samtools sort bamfilename.bam sortedbamfilename.bam  
samtools index sortedbamfilename.bam  

The sorted files can then be used in a genome viewer such as IGV as normal.


##NOTES
Pindel2BAM assumes that you have at least [samtools](https://github.com/samtools/samtools) version 1.5.
Currently, filler data exists for the following SAM fields:
* FLAG = 2
* MAPQ = *
* RNEXT = *
* PNEXT = 0
* TLEN = 0
* QUAL = *  

If the converter pin2sam has not been compiled, then Pindel2BAM will
compile it automatically.  

If your bam file names listed in the config file have a directory prefix,
then the / will be turned into _ for the output file names.  

If output files already exist, Pindel2SAM will append any data
within the _D and _SI files to the existing output .sam files.
You must therefore clean out the output directory before running
if you want fresh conversions.
