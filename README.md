# VSL_Interpreter

Program ::=  Instr; | Instr;Program
Instr ::=  m[x]<-0 | m[x]<-1 | m[x]<-m[y] | m[x]<-m[y]ANDm[z] | m[x]<-m[y]ORm[z] |  if B then Instr
B ::= m[x]=0 | m[x]=1 | m[x]=m[y]

where the meaning of instructions is:
m[x] <- 0               (in the memory cell with address x copy 0)
m[x] <- 1               (in the memory cell with address x copy 1)
m[x] <- m[y]            (in the memory cell with address x copy the element in the memory cell with address y)

m[x] <- m[y]ANDm[z]     
(in the memory cell with address x do the OR boolean operation with the element in the memory cell with address y and the element in the memorycell with address z)

m[x] <- m[y]ORm[z]
(in the memory cell with address x do the AND boolean operation with the element in the memory cell with address y and the element in the memorycell with address z)
