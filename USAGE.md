Basic usage
===========
Directory "examples" contains two sets of bacterial genomes. The easiest way 
to run "Sibelia" is to type:

	Sibelia -s loose <input FASTA file(s)>

For example:

	Sibelia -s loose Helicobacter_pylori.fasta

It will run "Sibelia" on the file "Helicobacter_pylori.fasta" with the "loose"
simplification parameters. There is another simplification parameters set,
called "fine". To run "Sibelia" on "Helicobacter_pylori.fasta" with "fine"
parameters set, type:

	Sibelia -s fine Helicobacter_pylori.fasta

The differenece between "loose" and "fine" set is that "loose" usually
produces fewer blocks, but longer. And it can lose some small synteny
blocks (< 5000 BP), while "fine" option produces more blocks (but shorter)
and their coverage is worse. Usually "loose" is the best choice, but if
you do not want to lose information about small-scale rearrangaments,
use "fine". You also can set simplification parameters manually 
(see section "Fine tuning").

You also may be interested in blocks that occur exactly once in each input
sequence, such blocks are used as input for MGRA algorithm for example. To get
such output use option "-a":

	Sibelia -s loose -a Helicobacter_pylori.fasta

Synteny blocks are also can be visualized with "Circos" [3]. Genomes from the
"examples" dir were taken from [4, 5].

Output description
==================
By default, "Sibelia" produces 3 files: 

1. Blocks coordinates
2. Genomes represented as permutations of the synteny blocks
3. Coverage report

There are also optional output files:

1. Sequences file
2. Files for generating a "Circos" picture
3. Dot file for resulting de Bruijn graph

All these files are described below in details.

Blocks coordinates
------------------
Default file name = "block_coords.txt". You can change it by setting cmd
parameter:

	-c <file name> or --coordsfile <file name>

First part of this file lists input chromosomes, their IDs, sizes and
descriptions. IDs are just sequence numbers of chromosomes (in the same
order as they apper in input files).

Second part of the file describes synteny blocks in sections separated by 
dashes. Each section starts with the synteny block ID. Then all instances
of this block are listed in tabular format. Each row of a table depicts
one instance of this synteny block. Columns of the table designate
following:

1. Chr_id -- ID of the chromosome, that the instance belongs to
2. Strand -- strand of the synteny block instance, either '+' or '-'. Input
sequences are treated as positive strands of the chromosomes
3. Start -- zero based index of the starting base pair of the instance. All
indices are given relative to POSITIVE strand
4. End -- zero based index of the base pair following last base pair of the
instance
5. Length -- length of the instance of the synteny block

Genomes permutations
--------------------
Default file name = "genomes_permutations.txt". You can change it by setting
cmd parameter:

	-p <file name> or --permfile <file name>

This file contains input chromosomes represented as permutations of the synteny
block. It has two lines for each input chromosome:

1. Header line -- FASTA header of the sequence (starting with '>')
2. Genome line -- sequence of synteny blocks instances as they appear on the 
positive strand of the chromosome. Each instance is represented as a signed
integer, where '+' sign depicts direct version of the block, and '-' depicts
reversed block

Coverage report
---------------
Default file name = "coverage_report.txt". You can change it by setting
cmd parameter:
	
	-r <file name> or  --reportfile <file name>

The file describes portion of the genomes, that found synteny block cover.
First part of the file describes input chromosomes (see "Blocks coordinates"
section). Second part of the file is a table with following collumns:

1. Degree -- multiplicity of the synteny block. For example, if synteny block
has degree = 3, then the are three instances of this block in the input
chromosomes
2. Count -- number of synteny blocks with a given degree
3. Total -- portion of all the input chromosomes that cover blocks with a given
degree
4. Chr <n> -- portion of the chromosome with id <n> that cover blocks with a given
example

This table contains one row for each degree and one ("All") row for overall coverage.
For example (output from "Helicobacter_pylori.fasta"):

	Degree	Count	Total	Chr 1	Chr 2
	2	23	94.68%	96.40%	93.09%	
	All	23	94.68%	96.40%	93.09%	

It means that there are 23 blocks with degree = 2, i.e. 23 * 2 instances, and
they cover 94.68% of both genomes, 96.40% of Chr 1 and 93.09% of Chr2. Note 
that synteny blocks can overlap (by at most 5000 BP for loose parameter set),
so sum in each column may not equal to value at last row.

Sequences file
--------------
Default file name = not set. To output this file, set cmd parameter 

	-q <file name> or --sequncesfile <file name>

This FASTA file contains sequences of instances of the synteny block. Each
sequence has header in following format:

	>Seq="<description>",Strand=<sign>,Block_id=<block id>,Start=<x>,End=<y>

Where <description> is a header of the FASTA sequence where the block instance
is located. Other fields are described in section "Coordinates file".

"Circos" files
--------------
Default directory name = not set. To output these files, set cmd parameter

	-d <dir name> or --circosdir <dir name>

You can visualize synteny blocks with a colorful circular diagramm by using
the "Circos" software [3]. To achieve this, do following:

1. Download and install Circos software
2. Run "Sibelia" with -d options set
3. Run Circos

For example, to generate Circos diagramm for example "Helicobacter_pylori.fasta"
perform following:

1. Create directory "circos" in directory with the FASTA file
2. Run "Sibelia" with following parameters:

	Sibelia -s loose -d ./circos Helicobacter_pylori.fasta

3. Run circos in the "circos" directory

For example of such diagramms (generated from "Helicobacter_pylori.fastaUSAGE),
see "examples/Helicobacter_pylori/circos/circos.png". Note that the directory\
for circos files must exist before running the program. Also note that such
diagrams can become very piled with larger genomes. To overcome this, plot only
big blocks, see section "Minimum block size".

Resulting de Bruijn graph
-------------------------
Default file name = not set. To output this file, set cmd parameter

	-g <file name> or  --graphfile <file name>

If you are a curious person, you can also view condensed de Bruijn graph that
is used for generating synteny blocks. To understand the graph, see [1].

Fine tuning
===========
Here we will describe parameters that can affect computation results.

Outputting only shared blocks
-----------------------------
Default = not set. Add flag to cmd parameters to set:

	-a or --sharedonly

Output only blocks that occur exactly once in each input sequence.

Parameters set
--------------
Our approach is parameter dependent. To understand the details, please see [1].
It incorporates several stages of computations. Each stage has two parameters,
K and D. There are two default parameters set, "loose" and "fine", see section
"Basic usage". But you can vary these parameters manually.

Using custom parameters set
---------------------------
<To be written>

Minimum block size
------------------
Default value = 5000. To change this value, set cmd parameter:

	-m <integer> or --minblocksize <integer>

If you are interested only in big synteny blocks, like > 100 000 BP, set
this parameter to an appropriate value.

Maximum number of iterations
----------------------------
Default value = 4. Tho change this value, set cmd parameter:

	-i <integer> or --maxiterations <integer>

Maximum number of iterations during a stage of simplification. Increasing
this parameter may slightly increase coverage.

References
==========
1. Ilya Minkin, Nikolay Vyahhi, Son Pham. "SyntenyFinder: A Synteny Blocks 
Generation and Genome Comparison Tool" (poster), WABI 2012
http://bioinf.spbau.ru/sites/default/files/SyntenyFinder.pdf
2. Max A. Alekseyev and Pavel A. Pevzner. "Breakpoint graphs and ancestral
genome reconstructions", Genome Res. 2009. 19: 943-957.
3. Circos. http://circos.ca
4. Helicobacter pylori. http://www.ncbi.nlm.nih.gov/genome/169
5. Staphylococcus aureus. http://www.ncbi.nlm.nih.gov/genome/154