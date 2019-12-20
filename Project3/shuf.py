import argparse, random, sys, string
from optparse import OptionParser

class shuffle:
    #list: the list of strings that need to be randomized
    #count: number of lines to output
    #repeat: boolean whether or not we can repeat lines
    def __init__(self, list, count, repeat):
        self.list = list
        self.count = count
        self.repeat = repeat

    def print_lines(self):

        if len(self.list) == 0:
            return

        if self.repeat:
            if self.count == sys.maxsize:
                while True:
                    sys.stdout.write(random.choice(self.list))
            else:
                while self.count > 0:
                    sys.stdout.write(random.choice(self.list))
                    self.count -= 1

        else:
            random.shuffle(self.list)
            if(self.count > len(self.list)):
                self.count=len(self.list)

            for i in range(0, self.count):
                sys.stdout.write(self.list[i])
        
def main():
    version_msg= "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE

Write a random permutation of the input lines to standard output."""
    #Set up basic stuff
    parser = OptionParser(version=version_msg,usage=usage_msg)

    
    #Implement --input-range (-i)
    #Act as if input is lo - hi, where each line is an integer
    parser.add_option("-i","--input-range", action="store",
                      dest="range",default="",
                      help="treat each number LO through HI as an input line")
    
    #Implement --head-count (-n)
    parser.add_option("-n","--head-count",action="store",dest="count",default=sys.maxsize,help="output at most COUNT lines")
    
    #Implement --repeat (-r)
    parser.add_option("-r","--repeat",action="store_true",dest="repeat",default=False,help="output lines can be repeated")
    
    (options, args) = parser.parse_args(sys.argv[1:])
    
    #Support not giving a file, but given "-" or nothing means
    #to read from standard input
    
    #-r implementation
    repeat = options.repeat

    #-n implementation
    try:
        count = int(options.count)
    except ValueError as e:
        parser.error("invalid count: '{0}'".format(options.count))

    if count < 0:
        parser.error("negative count '{0}'".format(options.count))

    #-i implementation
    range = options.range

    #check if option i was implemented
    if range != "":

        #Check that there is no args for the script
        if len(args) !=0:
            parser.error("extra arg: '{0}'".format(args[0]))

        #Look for the dash
        try:
            dashIndex = range.index('-')
        except ValueError as e:
            parser.error("invalid input range: '{0}'".format(options.range))
        if dashIndex == 0:
            parser.error("invalid input range: '{0}'".format(options.range))

        #Try and separate the ints
        loString, hiString = range.split("-")
        try:
            lo = int(loString)
            hi = int(hiString)
        except ValueError as e:
            parser.error("invalid input range: '{0}'".format(options.range))
        if lo > hi:
            parser.error("invalid input range: '{0}'".format(options.range))
        list = list(range(lo,hi+1))

        for i in range(len(list)):
            list[i] = str(list[i]+"\n")

    #if i is not specified
    else:
        #If there is an arg, take input from that
        if len(args) == 1:
            try:
                f = open(args[0], 'r')
                list = f.readlines()
                f.close()
            except IOError as e:
                errno, strerror = e.args
                parser.error("I/O error ({0}): {1}".format(errno,strerror))
        elif (len(args) == 0) or (len(args)==1 and args[0]=="-"):
            list = sys.stdin.readlines()
            sys.stdout.write("\n")
        else: #Only 1 operand allowed
            parser.error("extra operand '{0}'".format(args[1]))

    if len(list) != 0:
        list[len(list)-1] +="\n" #so its formatted correctly

    output = shuffle(list, count, repeat)
    output.print_lines()
            
if __name__ == "__main__":
    main()
