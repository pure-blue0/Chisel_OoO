
./thirdParty/benchmark/dhrystone.riscv:     file format elf64-littleriscv


Disassembly of section .text.init:

0000000080000000 <_start>:
    80000000:	4081                	li	ra,0
    80000002:	4101                	li	sp,0
    80000004:	4181                	li	gp,0
    80000006:	4201                	li	tp,0
    80000008:	4281                	li	t0,0
    8000000a:	4301                	li	t1,0
    8000000c:	4381                	li	t2,0
    8000000e:	4401                	li	s0,0
    80000010:	4481                	li	s1,0
    80000012:	4501                	li	a0,0
    80000014:	4581                	li	a1,0
    80000016:	4601                	li	a2,0
    80000018:	4681                	li	a3,0
    8000001a:	4701                	li	a4,0
    8000001c:	4781                	li	a5,0
    8000001e:	4801                	li	a6,0
    80000020:	4881                	li	a7,0
    80000022:	4901                	li	s2,0
    80000024:	4981                	li	s3,0
    80000026:	4a01                	li	s4,0
    80000028:	4a81                	li	s5,0
    8000002a:	4b01                	li	s6,0
    8000002c:	4b81                	li	s7,0
    8000002e:	4c01                	li	s8,0
    80000030:	4c81                	li	s9,0
    80000032:	4d01                	li	s10,0
    80000034:	4d81                	li	s11,0
    80000036:	4e01                	li	t3,0
    80000038:	4e81                	li	t4,0
    8000003a:	4f01                	li	t5,0
    8000003c:	4f81                	li	t6,0
    8000003e:	62f9                	lui	t0,0x1e
    80000040:	3002a073          	csrs	mstatus,t0
    80000044:	4285                	li	t0,1
    80000046:	02fe                	slli	t0,t0,0x1f
    80000048:	0002d863          	bgez	t0,80000058 <_start+0x58>
    8000004c:	4505                	li	a0,1
    8000004e:	00001297          	auipc	t0,0x1
    80000052:	faa2a923          	sw	a0,-78(t0) # 80001000 <tohost>
    80000056:	bfdd                	j	8000004c <_start+0x4c>
    80000058:	00000297          	auipc	t0,0x0
    8000005c:	03c28293          	addi	t0,t0,60 # 80000094 <trap_entry>
    80000060:	30529073          	csrw	mtvec,t0
    80000064:	00004197          	auipc	gp,0x4
    80000068:	f9818193          	addi	gp,gp,-104 # 80003ffc <__global_pointer$>
    8000006c:	00006217          	auipc	tp,0x6
    80000070:	05320213          	addi	tp,tp,83 # 800060bf <_end+0x3f>
    80000074:	fc027213          	andi	tp,tp,-64
    80000078:	f1402573          	csrr	a0,mhartid
    8000007c:	4585                	li	a1,1
    8000007e:	00b57063          	bgeu	a0,a1,8000007e <_start+0x7e>
    80000082:	00150113          	addi	sp,a0,1
    80000086:	0146                	slli	sp,sp,0x11
    80000088:	9112                	add	sp,sp,tp
    8000008a:	01151613          	slli	a2,a0,0x11
    8000008e:	9232                	add	tp,tp,a2
    80000090:	72c0206f          	j	800027bc <_init>

0000000080000094 <trap_entry>:
    80000094:	716d                	addi	sp,sp,-272
    80000096:	e406                	sd	ra,8(sp)
    80000098:	e80a                	sd	sp,16(sp)
    8000009a:	ec0e                	sd	gp,24(sp)
    8000009c:	f012                	sd	tp,32(sp)
    8000009e:	f416                	sd	t0,40(sp)
    800000a0:	f81a                	sd	t1,48(sp)
    800000a2:	fc1e                	sd	t2,56(sp)
    800000a4:	e0a2                	sd	s0,64(sp)
    800000a6:	e4a6                	sd	s1,72(sp)
    800000a8:	e8aa                	sd	a0,80(sp)
    800000aa:	ecae                	sd	a1,88(sp)
    800000ac:	f0b2                	sd	a2,96(sp)
    800000ae:	f4b6                	sd	a3,104(sp)
    800000b0:	f8ba                	sd	a4,112(sp)
    800000b2:	fcbe                	sd	a5,120(sp)
    800000b4:	e142                	sd	a6,128(sp)
    800000b6:	e546                	sd	a7,136(sp)
    800000b8:	e94a                	sd	s2,144(sp)
    800000ba:	ed4e                	sd	s3,152(sp)
    800000bc:	f152                	sd	s4,160(sp)
    800000be:	f556                	sd	s5,168(sp)
    800000c0:	f95a                	sd	s6,176(sp)
    800000c2:	fd5e                	sd	s7,184(sp)
    800000c4:	e1e2                	sd	s8,192(sp)
    800000c6:	e5e6                	sd	s9,200(sp)
    800000c8:	e9ea                	sd	s10,208(sp)
    800000ca:	edee                	sd	s11,216(sp)
    800000cc:	f1f2                	sd	t3,224(sp)
    800000ce:	f5f6                	sd	t4,232(sp)
    800000d0:	f9fa                	sd	t5,240(sp)
    800000d2:	fdfe                	sd	t6,248(sp)
    800000d4:	34202573          	csrr	a0,mcause
    800000d8:	341025f3          	csrr	a1,mepc
    800000dc:	860a                	mv	a2,sp
    800000de:	520020ef          	jal	ra,800025fe <handle_trap>
    800000e2:	34151073          	csrw	mepc,a0
    800000e6:	6289                	lui	t0,0x2
    800000e8:	8002829b          	addiw	t0,t0,-2048
    800000ec:	3002a073          	csrs	mstatus,t0
    800000f0:	60a2                	ld	ra,8(sp)
    800000f2:	6142                	ld	sp,16(sp)
    800000f4:	61e2                	ld	gp,24(sp)
    800000f6:	7202                	ld	tp,32(sp)
    800000f8:	72a2                	ld	t0,40(sp)
    800000fa:	7342                	ld	t1,48(sp)
    800000fc:	73e2                	ld	t2,56(sp)
    800000fe:	6406                	ld	s0,64(sp)
    80000100:	64a6                	ld	s1,72(sp)
    80000102:	6546                	ld	a0,80(sp)
    80000104:	65e6                	ld	a1,88(sp)
    80000106:	7606                	ld	a2,96(sp)
    80000108:	76a6                	ld	a3,104(sp)
    8000010a:	7746                	ld	a4,112(sp)
    8000010c:	77e6                	ld	a5,120(sp)
    8000010e:	680a                	ld	a6,128(sp)
    80000110:	68aa                	ld	a7,136(sp)
    80000112:	694a                	ld	s2,144(sp)
    80000114:	69ea                	ld	s3,152(sp)
    80000116:	7a0a                	ld	s4,160(sp)
    80000118:	7aaa                	ld	s5,168(sp)
    8000011a:	7b4a                	ld	s6,176(sp)
    8000011c:	7bea                	ld	s7,184(sp)
    8000011e:	6c0e                	ld	s8,192(sp)
    80000120:	6cae                	ld	s9,200(sp)
    80000122:	6d4e                	ld	s10,208(sp)
    80000124:	6dee                	ld	s11,216(sp)
    80000126:	7e0e                	ld	t3,224(sp)
    80000128:	7eae                	ld	t4,232(sp)
    8000012a:	7f4e                	ld	t5,240(sp)
    8000012c:	7fee                	ld	t6,248(sp)
    8000012e:	6151                	addi	sp,sp,272
    80000130:	30200073          	mret
	...

Disassembly of section .tohost:

0000000080001000 <tohost>:
	...

0000000080001040 <fromhost>:
	...

Disassembly of section .text:

0000000080002000 <Proc_7>:
    80002000:	2509                	addiw	a0,a0,2
    80002002:	9da9                	addw	a1,a1,a0
    80002004:	c20c                	sw	a1,0(a2)
    80002006:	8082                	ret

0000000080002008 <Proc_8>:
    80002008:	0056079b          	addiw	a5,a2,5
    8000200c:	0c800713          	li	a4,200
    80002010:	02e78733          	mul	a4,a5,a4
    80002014:	883e                	mv	a6,a5
    80002016:	060a                	slli	a2,a2,0x2
    80002018:	078a                	slli	a5,a5,0x2
    8000201a:	953e                	add	a0,a0,a5
    8000201c:	c114                	sw	a3,0(a0)
    8000201e:	c154                	sw	a3,4(a0)
    80002020:	07052c23          	sw	a6,120(a0)
    80002024:	00c707b3          	add	a5,a4,a2
    80002028:	97ae                	add	a5,a5,a1
    8000202a:	4b94                	lw	a3,16(a5)
    8000202c:	0107aa23          	sw	a6,20(a5)
    80002030:	0107ac23          	sw	a6,24(a5)
    80002034:	2685                	addiw	a3,a3,1
    80002036:	cb94                	sw	a3,16(a5)
    80002038:	4114                	lw	a3,0(a0)
    8000203a:	95ba                	add	a1,a1,a4
    8000203c:	95b2                	add	a1,a1,a2
    8000203e:	6785                	lui	a5,0x1
    80002040:	97ae                	add	a5,a5,a1
    80002042:	fad7aa23          	sw	a3,-76(a5) # fb4 <buflen.1+0xf74>
    80002046:	4795                	li	a5,5
    80002048:	00001717          	auipc	a4,0x1
    8000204c:	7ef72823          	sw	a5,2032(a4) # 80003838 <Int_Glob>
    80002050:	8082                	ret

0000000080002052 <Func_1>:
    80002052:	0ff57513          	andi	a0,a0,255
    80002056:	0ff5f593          	andi	a1,a1,255
    8000205a:	00b50463          	beq	a0,a1,80002062 <Func_1+0x10>
    8000205e:	4501                	li	a0,0
    80002060:	8082                	ret
    80002062:	00001797          	auipc	a5,0x1
    80002066:	7ca787a3          	sb	a0,1999(a5) # 80003831 <Ch_1_Glob>
    8000206a:	4505                	li	a0,1
    8000206c:	8082                	ret

000000008000206e <Func_2>:
    8000206e:	1101                	addi	sp,sp,-32
    80002070:	e822                	sd	s0,16(sp)
    80002072:	e426                	sd	s1,8(sp)
    80002074:	ec06                	sd	ra,24(sp)
    80002076:	842a                	mv	s0,a0
    80002078:	84ae                	mv	s1,a1
    8000207a:	0034c583          	lbu	a1,3(s1)
    8000207e:	00244503          	lbu	a0,2(s0)
    80002082:	fd1ff0ef          	jal	ra,80002052 <Func_1>
    80002086:	2501                	sext.w	a0,a0
    80002088:	f96d                	bnez	a0,8000207a <Func_2+0xc>
    8000208a:	85a6                	mv	a1,s1
    8000208c:	8522                	mv	a0,s0
    8000208e:	02d000ef          	jal	ra,800028ba <strcmp>
    80002092:	4781                	li	a5,0
    80002094:	00a05863          	blez	a0,800020a4 <Func_2+0x36>
    80002098:	47a9                	li	a5,10
    8000209a:	00001717          	auipc	a4,0x1
    8000209e:	78f72f23          	sw	a5,1950(a4) # 80003838 <Int_Glob>
    800020a2:	4785                	li	a5,1
    800020a4:	60e2                	ld	ra,24(sp)
    800020a6:	6442                	ld	s0,16(sp)
    800020a8:	64a2                	ld	s1,8(sp)
    800020aa:	853e                	mv	a0,a5
    800020ac:	6105                	addi	sp,sp,32
    800020ae:	8082                	ret

00000000800020b0 <Func_3>:
    800020b0:	1579                	addi	a0,a0,-2
    800020b2:	00153513          	seqz	a0,a0
    800020b6:	8082                	ret

00000000800020b8 <Proc_6>:
    800020b8:	1101                	addi	sp,sp,-32
    800020ba:	e822                	sd	s0,16(sp)
    800020bc:	e426                	sd	s1,8(sp)
    800020be:	842a                	mv	s0,a0
    800020c0:	ec06                	sd	ra,24(sp)
    800020c2:	84ae                	mv	s1,a1
    800020c4:	fedff0ef          	jal	ra,800020b0 <Func_3>
    800020c8:	87a2                	mv	a5,s0
    800020ca:	e111                	bnez	a0,800020ce <Proc_6+0x16>
    800020cc:	478d                	li	a5,3
    800020ce:	c09c                	sw	a5,0(s1)
    800020d0:	4789                	li	a5,2
    800020d2:	02f40d63          	beq	s0,a5,8000210c <Proc_6+0x54>
    800020d6:	0287e263          	bltu	a5,s0,800020fa <Proc_6+0x42>
    800020da:	c809                	beqz	s0,800020ec <Proc_6+0x34>
    800020dc:	00001717          	auipc	a4,0x1
    800020e0:	75c72703          	lw	a4,1884(a4) # 80003838 <Int_Glob>
    800020e4:	06400793          	li	a5,100
    800020e8:	02e7d963          	bge	a5,a4,8000211a <Proc_6+0x62>
    800020ec:	0004a023          	sw	zero,0(s1)
    800020f0:	60e2                	ld	ra,24(sp)
    800020f2:	6442                	ld	s0,16(sp)
    800020f4:	64a2                	ld	s1,8(sp)
    800020f6:	6105                	addi	sp,sp,32
    800020f8:	8082                	ret
    800020fa:	4711                	li	a4,4
    800020fc:	fee41ae3          	bne	s0,a4,800020f0 <Proc_6+0x38>
    80002100:	60e2                	ld	ra,24(sp)
    80002102:	6442                	ld	s0,16(sp)
    80002104:	c09c                	sw	a5,0(s1)
    80002106:	64a2                	ld	s1,8(sp)
    80002108:	6105                	addi	sp,sp,32
    8000210a:	8082                	ret
    8000210c:	60e2                	ld	ra,24(sp)
    8000210e:	6442                	ld	s0,16(sp)
    80002110:	4785                	li	a5,1
    80002112:	c09c                	sw	a5,0(s1)
    80002114:	64a2                	ld	s1,8(sp)
    80002116:	6105                	addi	sp,sp,32
    80002118:	8082                	ret
    8000211a:	60e2                	ld	ra,24(sp)
    8000211c:	6442                	ld	s0,16(sp)
    8000211e:	478d                	li	a5,3
    80002120:	c09c                	sw	a5,0(s1)
    80002122:	64a2                	ld	s1,8(sp)
    80002124:	6105                	addi	sp,sp,32
    80002126:	8082                	ret

0000000080002128 <debug_printf>:
    80002128:	7139                	addi	sp,sp,-64
    8000212a:	e42e                	sd	a1,8(sp)
    8000212c:	e832                	sd	a2,16(sp)
    8000212e:	ec36                	sd	a3,24(sp)
    80002130:	f03a                	sd	a4,32(sp)
    80002132:	f43e                	sd	a5,40(sp)
    80002134:	f842                	sd	a6,48(sp)
    80002136:	fc46                	sd	a7,56(sp)
    80002138:	6121                	addi	sp,sp,64
    8000213a:	8082                	ret

000000008000213c <Proc_2>:
    8000213c:	00001717          	auipc	a4,0x1
    80002140:	6f574703          	lbu	a4,1781(a4) # 80003831 <Ch_1_Glob>
    80002144:	04100793          	li	a5,65
    80002148:	00f70363          	beq	a4,a5,8000214e <Proc_2+0x12>
    8000214c:	8082                	ret
    8000214e:	411c                	lw	a5,0(a0)
    80002150:	00001717          	auipc	a4,0x1
    80002154:	6e872703          	lw	a4,1768(a4) # 80003838 <Int_Glob>
    80002158:	27a5                	addiw	a5,a5,9
    8000215a:	9f99                	subw	a5,a5,a4
    8000215c:	c11c                	sw	a5,0(a0)
    8000215e:	8082                	ret

0000000080002160 <Proc_3>:
    80002160:	84c18793          	addi	a5,gp,-1972 # 80003848 <Ptr_Glob>
    80002164:	6390                	ld	a2,0(a5)
    80002166:	c601                	beqz	a2,8000216e <Proc_3+0xe>
    80002168:	6218                	ld	a4,0(a2)
    8000216a:	e118                	sd	a4,0(a0)
    8000216c:	6390                	ld	a2,0(a5)
    8000216e:	0641                	addi	a2,a2,16
    80002170:	00001597          	auipc	a1,0x1
    80002174:	6c85a583          	lw	a1,1736(a1) # 80003838 <Int_Glob>
    80002178:	4529                	li	a0,10
    8000217a:	b559                	j	80002000 <Proc_7>

000000008000217c <Proc_1>:
    8000217c:	1101                	addi	sp,sp,-32
    8000217e:	e04a                	sd	s2,0(sp)
    80002180:	84c18913          	addi	s2,gp,-1972 # 80003848 <Ptr_Glob>
    80002184:	00093783          	ld	a5,0(s2)
    80002188:	e822                	sd	s0,16(sp)
    8000218a:	6100                	ld	s0,0(a0)
    8000218c:	6398                	ld	a4,0(a5)
    8000218e:	0107b803          	ld	a6,16(a5)
    80002192:	6f8c                	ld	a1,24(a5)
    80002194:	7390                	ld	a2,32(a5)
    80002196:	7794                	ld	a3,40(a5)
    80002198:	e426                	sd	s1,8(sp)
    8000219a:	ec06                	sd	ra,24(sp)
    8000219c:	84aa                	mv	s1,a0
    8000219e:	6788                	ld	a0,8(a5)
    800021a0:	7b9c                	ld	a5,48(a5)
    800021a2:	e018                	sd	a4,0(s0)
    800021a4:	6098                	ld	a4,0(s1)
    800021a6:	01043823          	sd	a6,16(s0)
    800021aa:	e408                	sd	a0,8(s0)
    800021ac:	f81c                	sd	a5,48(s0)
    800021ae:	ec0c                	sd	a1,24(s0)
    800021b0:	4795                	li	a5,5
    800021b2:	f010                	sd	a2,32(s0)
    800021b4:	f414                	sd	a3,40(s0)
    800021b6:	c89c                	sw	a5,16(s1)
    800021b8:	c81c                	sw	a5,16(s0)
    800021ba:	e018                	sd	a4,0(s0)
    800021bc:	8522                	mv	a0,s0
    800021be:	fa3ff0ef          	jal	ra,80002160 <Proc_3>
    800021c2:	441c                	lw	a5,8(s0)
    800021c4:	cb95                	beqz	a5,800021f8 <Proc_1+0x7c>
    800021c6:	609c                	ld	a5,0(s1)
    800021c8:	60e2                	ld	ra,24(sp)
    800021ca:	6442                	ld	s0,16(sp)
    800021cc:	0007b883          	ld	a7,0(a5)
    800021d0:	0087b803          	ld	a6,8(a5)
    800021d4:	6b8c                	ld	a1,16(a5)
    800021d6:	6f90                	ld	a2,24(a5)
    800021d8:	7394                	ld	a3,32(a5)
    800021da:	7798                	ld	a4,40(a5)
    800021dc:	7b9c                	ld	a5,48(a5)
    800021de:	0114b023          	sd	a7,0(s1)
    800021e2:	0104b423          	sd	a6,8(s1)
    800021e6:	e88c                	sd	a1,16(s1)
    800021e8:	ec90                	sd	a2,24(s1)
    800021ea:	f094                	sd	a3,32(s1)
    800021ec:	f498                	sd	a4,40(s1)
    800021ee:	f89c                	sd	a5,48(s1)
    800021f0:	6902                	ld	s2,0(sp)
    800021f2:	64a2                	ld	s1,8(sp)
    800021f4:	6105                	addi	sp,sp,32
    800021f6:	8082                	ret
    800021f8:	44c8                	lw	a0,12(s1)
    800021fa:	4799                	li	a5,6
    800021fc:	00c40593          	addi	a1,s0,12
    80002200:	c81c                	sw	a5,16(s0)
    80002202:	eb7ff0ef          	jal	ra,800020b8 <Proc_6>
    80002206:	00093783          	ld	a5,0(s2)
    8000220a:	4808                	lw	a0,16(s0)
    8000220c:	01040613          	addi	a2,s0,16
    80002210:	639c                	ld	a5,0(a5)
    80002212:	60e2                	ld	ra,24(sp)
    80002214:	64a2                	ld	s1,8(sp)
    80002216:	e01c                	sd	a5,0(s0)
    80002218:	6442                	ld	s0,16(sp)
    8000221a:	6902                	ld	s2,0(sp)
    8000221c:	45a9                	li	a1,10
    8000221e:	6105                	addi	sp,sp,32
    80002220:	b3c5                	j	80002000 <Proc_7>

0000000080002222 <Proc_4>:
    80002222:	00001697          	auipc	a3,0x1
    80002226:	61268693          	addi	a3,a3,1554 # 80003834 <Bool_Glob>
    8000222a:	4298                	lw	a4,0(a3)
    8000222c:	00001797          	auipc	a5,0x1
    80002230:	6057c783          	lbu	a5,1541(a5) # 80003831 <Ch_1_Glob>
    80002234:	fbf78793          	addi	a5,a5,-65
    80002238:	0017b793          	seqz	a5,a5
    8000223c:	8fd9                	or	a5,a5,a4
    8000223e:	c29c                	sw	a5,0(a3)
    80002240:	04200793          	li	a5,66
    80002244:	00001717          	auipc	a4,0x1
    80002248:	5ef70623          	sb	a5,1516(a4) # 80003830 <Ch_2_Glob>
    8000224c:	8082                	ret

000000008000224e <Proc_5>:
    8000224e:	04100793          	li	a5,65
    80002252:	00001717          	auipc	a4,0x1
    80002256:	5cf70fa3          	sb	a5,1503(a4) # 80003831 <Ch_1_Glob>
    8000225a:	00001797          	auipc	a5,0x1
    8000225e:	5c07ad23          	sw	zero,1498(a5) # 80003834 <Bool_Glob>
    80002262:	8082                	ret

0000000080002264 <vprintfmt>:
    80002264:	710d                	addi	sp,sp,-352
    80002266:	eaa2                	sd	s0,336(sp)
    80002268:	e6a6                	sd	s1,328(sp)
    8000226a:	e2ca                	sd	s2,320(sp)
    8000226c:	f656                	sd	s5,296(sp)
    8000226e:	f25a                	sd	s6,288(sp)
    80002270:	ee5e                	sd	s7,280(sp)
    80002272:	ee86                	sd	ra,344(sp)
    80002274:	fe4e                	sd	s3,312(sp)
    80002276:	fa52                	sd	s4,304(sp)
    80002278:	ea62                	sd	s8,272(sp)
    8000227a:	e666                	sd	s9,264(sp)
    8000227c:	e26a                	sd	s10,256(sp)
    8000227e:	892a                	mv	s2,a0
    80002280:	84ae                	mv	s1,a1
    80002282:	8432                	mv	s0,a2
    80002284:	8b36                	mv	s6,a3
    80002286:	02500a93          	li	s5,37
    8000228a:	00001b97          	auipc	s7,0x1
    8000228e:	41ab8b93          	addi	s7,s7,1050 # 800036a4 <main+0xd4c>
    80002292:	a029                	j	8000229c <vprintfmt+0x38>
    80002294:	c131                	beqz	a0,800022d8 <vprintfmt+0x74>
    80002296:	85a6                	mv	a1,s1
    80002298:	0405                	addi	s0,s0,1
    8000229a:	9902                	jalr	s2
    8000229c:	00044503          	lbu	a0,0(s0)
    800022a0:	ff551ae3          	bne	a0,s5,80002294 <vprintfmt+0x30>
    800022a4:	00144683          	lbu	a3,1(s0)
    800022a8:	00140a13          	addi	s4,s0,1
    800022ac:	8752                	mv	a4,s4
    800022ae:	02000c93          	li	s9,32
    800022b2:	59fd                	li	s3,-1
    800022b4:	5d7d                	li	s10,-1
    800022b6:	4501                	li	a0,0
    800022b8:	05500613          	li	a2,85
    800022bc:	45a5                	li	a1,9
    800022be:	fdd6879b          	addiw	a5,a3,-35
    800022c2:	0ff7f793          	andi	a5,a5,255
    800022c6:	00170413          	addi	s0,a4,1
    800022ca:	04f66163          	bltu	a2,a5,8000230c <vprintfmt+0xa8>
    800022ce:	078a                	slli	a5,a5,0x2
    800022d0:	97de                	add	a5,a5,s7
    800022d2:	439c                	lw	a5,0(a5)
    800022d4:	97de                	add	a5,a5,s7
    800022d6:	8782                	jr	a5
    800022d8:	60f6                	ld	ra,344(sp)
    800022da:	6456                	ld	s0,336(sp)
    800022dc:	64b6                	ld	s1,328(sp)
    800022de:	6916                	ld	s2,320(sp)
    800022e0:	79f2                	ld	s3,312(sp)
    800022e2:	7a52                	ld	s4,304(sp)
    800022e4:	7ab2                	ld	s5,296(sp)
    800022e6:	7b12                	ld	s6,288(sp)
    800022e8:	6bf2                	ld	s7,280(sp)
    800022ea:	6c52                	ld	s8,272(sp)
    800022ec:	6cb2                	ld	s9,264(sp)
    800022ee:	6d12                	ld	s10,256(sp)
    800022f0:	6135                	addi	sp,sp,352
    800022f2:	8082                	ret
    800022f4:	8cb6                	mv	s9,a3
    800022f6:	00174683          	lbu	a3,1(a4)
    800022fa:	8722                	mv	a4,s0
    800022fc:	00170413          	addi	s0,a4,1
    80002300:	fdd6879b          	addiw	a5,a3,-35
    80002304:	0ff7f793          	andi	a5,a5,255
    80002308:	fcf673e3          	bgeu	a2,a5,800022ce <vprintfmt+0x6a>
    8000230c:	85a6                	mv	a1,s1
    8000230e:	02500513          	li	a0,37
    80002312:	9902                	jalr	s2
    80002314:	8452                	mv	s0,s4
    80002316:	b759                	j	8000229c <vprintfmt+0x38>
    80002318:	fd06899b          	addiw	s3,a3,-48
    8000231c:	00174683          	lbu	a3,1(a4)
    80002320:	8722                	mv	a4,s0
    80002322:	fd06879b          	addiw	a5,a3,-48
    80002326:	0006881b          	sext.w	a6,a3
    8000232a:	02f5e563          	bltu	a1,a5,80002354 <vprintfmt+0xf0>
    8000232e:	00174683          	lbu	a3,1(a4)
    80002332:	0029979b          	slliw	a5,s3,0x2
    80002336:	013787bb          	addw	a5,a5,s3
    8000233a:	0017979b          	slliw	a5,a5,0x1
    8000233e:	010787bb          	addw	a5,a5,a6
    80002342:	fd06889b          	addiw	a7,a3,-48
    80002346:	0705                	addi	a4,a4,1
    80002348:	fd07899b          	addiw	s3,a5,-48
    8000234c:	0006881b          	sext.w	a6,a3
    80002350:	fd15ffe3          	bgeu	a1,a7,8000232e <vprintfmt+0xca>
    80002354:	f60d55e3          	bgez	s10,800022be <vprintfmt+0x5a>
    80002358:	8d4e                	mv	s10,s3
    8000235a:	59fd                	li	s3,-1
    8000235c:	b78d                	j	800022be <vprintfmt+0x5a>
    8000235e:	00174683          	lbu	a3,1(a4)
    80002362:	8722                	mv	a4,s0
    80002364:	bfa9                	j	800022be <vprintfmt+0x5a>
    80002366:	85a6                	mv	a1,s1
    80002368:	02500513          	li	a0,37
    8000236c:	9902                	jalr	s2
    8000236e:	b73d                	j	8000229c <vprintfmt+0x38>
    80002370:	000b2983          	lw	s3,0(s6)
    80002374:	00174683          	lbu	a3,1(a4)
    80002378:	0b21                	addi	s6,s6,8
    8000237a:	8722                	mv	a4,s0
    8000237c:	bfe1                	j	80002354 <vprintfmt+0xf0>
    8000237e:	03000513          	li	a0,48
    80002382:	85a6                	mv	a1,s1
    80002384:	9902                	jalr	s2
    80002386:	85a6                	mv	a1,s1
    80002388:	07800513          	li	a0,120
    8000238c:	9902                	jalr	s2
    8000238e:	4741                	li	a4,16
    80002390:	008b0693          	addi	a3,s6,8
    80002394:	000b3783          	ld	a5,0(s6)
    80002398:	8b36                	mv	s6,a3
    8000239a:	02e7f9b3          	remu	s3,a5,a4
    8000239e:	2c81                	sext.w	s9,s9
    800023a0:	0054                	addi	a3,sp,4
    800023a2:	4c05                	li	s8,1
    800023a4:	c04e                	sw	s3,0(sp)
    800023a6:	14e7e463          	bltu	a5,a4,800024ee <vprintfmt+0x28a>
    800023aa:	02e7d7b3          	divu	a5,a5,a4
    800023ae:	0691                	addi	a3,a3,4
    800023b0:	8a62                	mv	s4,s8
    800023b2:	2c05                	addiw	s8,s8,1
    800023b4:	02e7f9b3          	remu	s3,a5,a4
    800023b8:	ff36ae23          	sw	s3,-4(a3)
    800023bc:	fee7f7e3          	bgeu	a5,a4,800023aa <vprintfmt+0x146>
    800023c0:	01ac5963          	bge	s8,s10,800023d2 <vprintfmt+0x16e>
    800023c4:	2d01                	sext.w	s10,s10
    800023c6:	3d7d                	addiw	s10,s10,-1
    800023c8:	85a6                	mv	a1,s1
    800023ca:	8566                	mv	a0,s9
    800023cc:	9902                	jalr	s2
    800023ce:	ffac4ce3          	blt	s8,s10,800023c6 <vprintfmt+0x162>
    800023d2:	0a0a                	slli	s4,s4,0x2
    800023d4:	9a0a                	add	s4,s4,sp
    800023d6:	8c8a                	mv	s9,sp
    800023d8:	4c25                	li	s8,9
    800023da:	a021                	j	800023e2 <vprintfmt+0x17e>
    800023dc:	ffca2983          	lw	s3,-4(s4)
    800023e0:	1a71                	addi	s4,s4,-4
    800023e2:	05700513          	li	a0,87
    800023e6:	013c6463          	bltu	s8,s3,800023ee <vprintfmt+0x18a>
    800023ea:	03000513          	li	a0,48
    800023ee:	85a6                	mv	a1,s1
    800023f0:	0135053b          	addw	a0,a0,s3
    800023f4:	9902                	jalr	s2
    800023f6:	ff9a13e3          	bne	s4,s9,800023dc <vprintfmt+0x178>
    800023fa:	b54d                	j	8000229c <vprintfmt+0x38>
    800023fc:	4785                	li	a5,1
    800023fe:	008b0a13          	addi	s4,s6,8
    80002402:	00a7c363          	blt	a5,a0,80002408 <vprintfmt+0x1a4>
    80002406:	c965                	beqz	a0,800024f6 <vprintfmt+0x292>
    80002408:	000b3983          	ld	s3,0(s6)
    8000240c:	0e09cc63          	bltz	s3,80002504 <vprintfmt+0x2a0>
    80002410:	87ce                	mv	a5,s3
    80002412:	8b52                	mv	s6,s4
    80002414:	4729                	li	a4,10
    80002416:	b751                	j	8000239a <vprintfmt+0x136>
    80002418:	00174683          	lbu	a3,1(a4)
    8000241c:	2505                	addiw	a0,a0,1
    8000241e:	8722                	mv	a4,s0
    80002420:	bd79                	j	800022be <vprintfmt+0x5a>
    80002422:	4741                	li	a4,16
    80002424:	4785                	li	a5,1
    80002426:	008b0693          	addi	a3,s6,8
    8000242a:	f6a7c5e3          	blt	a5,a0,80002394 <vprintfmt+0x130>
    8000242e:	f13d                	bnez	a0,80002394 <vprintfmt+0x130>
    80002430:	000b6783          	lwu	a5,0(s6)
    80002434:	8b36                	mv	s6,a3
    80002436:	b795                	j	8000239a <vprintfmt+0x136>
    80002438:	000b3a03          	ld	s4,0(s6)
    8000243c:	0b21                	addi	s6,s6,8
    8000243e:	0c0a0c63          	beqz	s4,80002516 <vprintfmt+0x2b2>
    80002442:	07a05f63          	blez	s10,800024c0 <vprintfmt+0x25c>
    80002446:	02d00793          	li	a5,45
    8000244a:	04fc9063          	bne	s9,a5,8000248a <vprintfmt+0x226>
    8000244e:	000a4503          	lbu	a0,0(s4)
    80002452:	c105                	beqz	a0,80002472 <vprintfmt+0x20e>
    80002454:	5c7d                	li	s8,-1
    80002456:	0009c563          	bltz	s3,80002460 <vprintfmt+0x1fc>
    8000245a:	39fd                	addiw	s3,s3,-1
    8000245c:	01898963          	beq	s3,s8,8000246e <vprintfmt+0x20a>
    80002460:	85a6                	mv	a1,s1
    80002462:	0a05                	addi	s4,s4,1
    80002464:	9902                	jalr	s2
    80002466:	000a4503          	lbu	a0,0(s4)
    8000246a:	3d7d                	addiw	s10,s10,-1
    8000246c:	f56d                	bnez	a0,80002456 <vprintfmt+0x1f2>
    8000246e:	e3a057e3          	blez	s10,8000229c <vprintfmt+0x38>
    80002472:	3d7d                	addiw	s10,s10,-1
    80002474:	85a6                	mv	a1,s1
    80002476:	02000513          	li	a0,32
    8000247a:	9902                	jalr	s2
    8000247c:	fe0d1be3          	bnez	s10,80002472 <vprintfmt+0x20e>
    80002480:	bd31                	j	8000229c <vprintfmt+0x38>
    80002482:	00001a17          	auipc	s4,0x1
    80002486:	1dea0a13          	addi	s4,s4,478 # 80003660 <main+0xd08>
    8000248a:	000d061b          	sext.w	a2,s10
    8000248e:	87d2                	mv	a5,s4
    80002490:	013a06b3          	add	a3,s4,s3
    80002494:	00099663          	bnez	s3,800024a0 <vprintfmt+0x23c>
    80002498:	a829                	j	800024b2 <vprintfmt+0x24e>
    8000249a:	0785                	addi	a5,a5,1
    8000249c:	00d78563          	beq	a5,a3,800024a6 <vprintfmt+0x242>
    800024a0:	0007c703          	lbu	a4,0(a5)
    800024a4:	fb7d                	bnez	a4,8000249a <vprintfmt+0x236>
    800024a6:	414787b3          	sub	a5,a5,s4
    800024aa:	40f60d3b          	subw	s10,a2,a5
    800024ae:	01a05963          	blez	s10,800024c0 <vprintfmt+0x25c>
    800024b2:	2c81                	sext.w	s9,s9
    800024b4:	3d7d                	addiw	s10,s10,-1
    800024b6:	85a6                	mv	a1,s1
    800024b8:	8566                	mv	a0,s9
    800024ba:	9902                	jalr	s2
    800024bc:	fe0d1ce3          	bnez	s10,800024b4 <vprintfmt+0x250>
    800024c0:	000a4503          	lbu	a0,0(s4)
    800024c4:	dc050ce3          	beqz	a0,8000229c <vprintfmt+0x38>
    800024c8:	5c7d                	li	s8,-1
    800024ca:	b771                	j	80002456 <vprintfmt+0x1f2>
    800024cc:	fffd4793          	not	a5,s10
    800024d0:	97fd                	srai	a5,a5,0x3f
    800024d2:	00fd77b3          	and	a5,s10,a5
    800024d6:	00174683          	lbu	a3,1(a4)
    800024da:	00078d1b          	sext.w	s10,a5
    800024de:	8722                	mv	a4,s0
    800024e0:	bbf9                	j	800022be <vprintfmt+0x5a>
    800024e2:	000b2503          	lw	a0,0(s6)
    800024e6:	85a6                	mv	a1,s1
    800024e8:	0b21                	addi	s6,s6,8
    800024ea:	9902                	jalr	s2
    800024ec:	bb45                	j	8000229c <vprintfmt+0x38>
    800024ee:	4a01                	li	s4,0
    800024f0:	edac4ae3          	blt	s8,s10,800023c4 <vprintfmt+0x160>
    800024f4:	bdf9                	j	800023d2 <vprintfmt+0x16e>
    800024f6:	000b2983          	lw	s3,0(s6)
    800024fa:	bf09                	j	8000240c <vprintfmt+0x1a8>
    800024fc:	4721                	li	a4,8
    800024fe:	b71d                	j	80002424 <vprintfmt+0x1c0>
    80002500:	4729                	li	a4,10
    80002502:	b70d                	j	80002424 <vprintfmt+0x1c0>
    80002504:	85a6                	mv	a1,s1
    80002506:	02d00513          	li	a0,45
    8000250a:	9902                	jalr	s2
    8000250c:	8b52                	mv	s6,s4
    8000250e:	413007b3          	neg	a5,s3
    80002512:	4729                	li	a4,10
    80002514:	b559                	j	8000239a <vprintfmt+0x136>
    80002516:	01a05663          	blez	s10,80002522 <vprintfmt+0x2be>
    8000251a:	02d00793          	li	a5,45
    8000251e:	f6fc92e3          	bne	s9,a5,80002482 <vprintfmt+0x21e>
    80002522:	00001a17          	auipc	s4,0x1
    80002526:	13ea0a13          	addi	s4,s4,318 # 80003660 <main+0xd08>
    8000252a:	02800513          	li	a0,40
    8000252e:	5c7d                	li	s8,-1
    80002530:	b71d                	j	80002456 <vprintfmt+0x1f2>

0000000080002532 <sprintf_putch.0>:
    80002532:	619c                	ld	a5,0(a1)
    80002534:	00a78023          	sb	a0,0(a5)
    80002538:	619c                	ld	a5,0(a1)
    8000253a:	0785                	addi	a5,a5,1
    8000253c:	e19c                	sd	a5,0(a1)
    8000253e:	8082                	ret

0000000080002540 <putchar>:
    80002540:	04022803          	lw	a6,64(tp) # 40 <buflen.1>
    80002544:	00020793          	mv	a5,tp
    80002548:	97c2                	add	a5,a5,a6
    8000254a:	7159                	addi	sp,sp,-112
    8000254c:	0018069b          	addiw	a3,a6,1
    80002550:	04d22023          	sw	a3,64(tp) # 40 <buflen.1>
    80002554:	00a78023          	sb	a0,0(a5)
    80002558:	03f10713          	addi	a4,sp,63
    8000255c:	47a9                	li	a5,10
    8000255e:	fc077713          	andi	a4,a4,-64
    80002562:	00f50963          	beq	a0,a5,80002574 <putchar+0x34>
    80002566:	04000793          	li	a5,64
    8000256a:	00f68563          	beq	a3,a5,80002574 <putchar+0x34>
    8000256e:	4501                	li	a0,0
    80002570:	6165                	addi	sp,sp,112
    80002572:	8082                	ret
    80002574:	04000793          	li	a5,64
    80002578:	e31c                	sd	a5,0(a4)
    8000257a:	4785                	li	a5,1
    8000257c:	e71c                	sd	a5,8(a4)
    8000257e:	00020613          	mv	a2,tp
    80002582:	eb10                	sd	a2,16(a4)
    80002584:	ef14                	sd	a3,24(a4)
    80002586:	0ff0000f          	fence
    8000258a:	fffff697          	auipc	a3,0xfffff
    8000258e:	ab668693          	addi	a3,a3,-1354 # 80001040 <fromhost>
    80002592:	fffff797          	auipc	a5,0xfffff
    80002596:	a6e7b723          	sd	a4,-1426(a5) # 80001000 <tohost>
    8000259a:	629c                	ld	a5,0(a3)
    8000259c:	dffd                	beqz	a5,8000259a <putchar+0x5a>
    8000259e:	fffff797          	auipc	a5,0xfffff
    800025a2:	aa07b123          	sd	zero,-1374(a5) # 80001040 <fromhost>
    800025a6:	0ff0000f          	fence
    800025aa:	04022023          	sw	zero,64(tp) # 40 <buflen.1>
    800025ae:	631c                	ld	a5,0(a4)
    800025b0:	4501                	li	a0,0
    800025b2:	6165                	addi	sp,sp,112
    800025b4:	8082                	ret

00000000800025b6 <setStats>:
    800025b6:	b0002773          	csrr	a4,mcycle
    800025ba:	00004797          	auipc	a5,0x4
    800025be:	a6e78793          	addi	a5,a5,-1426 # 80006028 <counters>
    800025c2:	e901                	bnez	a0,800025d2 <setStats+0x1c>
    800025c4:	6394                	ld	a3,0(a5)
    800025c6:	00001617          	auipc	a2,0x1
    800025ca:	0a260613          	addi	a2,a2,162 # 80003668 <main+0xd10>
    800025ce:	eb90                	sd	a2,16(a5)
    800025d0:	8f15                	sub	a4,a4,a3
    800025d2:	e398                	sd	a4,0(a5)
    800025d4:	b0202773          	csrr	a4,minstret
    800025d8:	e901                	bnez	a0,800025e8 <setStats+0x32>
    800025da:	6794                	ld	a3,8(a5)
    800025dc:	00001617          	auipc	a2,0x1
    800025e0:	09460613          	addi	a2,a2,148 # 80003670 <main+0xd18>
    800025e4:	ef90                	sd	a2,24(a5)
    800025e6:	8f15                	sub	a4,a4,a3
    800025e8:	e798                	sd	a4,8(a5)
    800025ea:	8082                	ret

00000000800025ec <tohost_exit>:
    800025ec:	00151793          	slli	a5,a0,0x1
    800025f0:	0017e793          	ori	a5,a5,1
    800025f4:	fffff717          	auipc	a4,0xfffff
    800025f8:	a0f73623          	sd	a5,-1524(a4) # 80001000 <tohost>
    800025fc:	a001                	j	800025fc <tohost_exit+0x10>

00000000800025fe <handle_trap>:
    800025fe:	6785                	lui	a5,0x1
    80002600:	a7378793          	addi	a5,a5,-1421 # a73 <buflen.1+0xa33>
    80002604:	fffff717          	auipc	a4,0xfffff
    80002608:	9ef73e23          	sd	a5,-1540(a4) # 80001000 <tohost>
    8000260c:	a001                	j	8000260c <handle_trap+0xe>

000000008000260e <exit>:
    8000260e:	1141                	addi	sp,sp,-16
    80002610:	e406                	sd	ra,8(sp)
    80002612:	fdbff0ef          	jal	ra,800025ec <tohost_exit>

0000000080002616 <abort>:
    80002616:	10d00793          	li	a5,269
    8000261a:	fffff717          	auipc	a4,0xfffff
    8000261e:	9ef73323          	sd	a5,-1562(a4) # 80001000 <tohost>
    80002622:	a001                	j	80002622 <abort+0xc>

0000000080002624 <printstr>:
    80002624:	00054783          	lbu	a5,0(a0)
    80002628:	7159                	addi	sp,sp,-112
    8000262a:	03f10693          	addi	a3,sp,63
    8000262e:	fc06f693          	andi	a3,a3,-64
    80002632:	c3b9                	beqz	a5,80002678 <printstr+0x54>
    80002634:	87aa                	mv	a5,a0
    80002636:	0017c703          	lbu	a4,1(a5)
    8000263a:	0785                	addi	a5,a5,1
    8000263c:	ff6d                	bnez	a4,80002636 <printstr+0x12>
    8000263e:	8f89                	sub	a5,a5,a0
    80002640:	04000713          	li	a4,64
    80002644:	e298                	sd	a4,0(a3)
    80002646:	4705                	li	a4,1
    80002648:	e698                	sd	a4,8(a3)
    8000264a:	ea88                	sd	a0,16(a3)
    8000264c:	ee9c                	sd	a5,24(a3)
    8000264e:	0ff0000f          	fence
    80002652:	fffff717          	auipc	a4,0xfffff
    80002656:	9ee70713          	addi	a4,a4,-1554 # 80001040 <fromhost>
    8000265a:	fffff797          	auipc	a5,0xfffff
    8000265e:	9ad7b323          	sd	a3,-1626(a5) # 80001000 <tohost>
    80002662:	631c                	ld	a5,0(a4)
    80002664:	dffd                	beqz	a5,80002662 <printstr+0x3e>
    80002666:	fffff797          	auipc	a5,0xfffff
    8000266a:	9c07bd23          	sd	zero,-1574(a5) # 80001040 <fromhost>
    8000266e:	0ff0000f          	fence
    80002672:	629c                	ld	a5,0(a3)
    80002674:	6165                	addi	sp,sp,112
    80002676:	8082                	ret
    80002678:	4781                	li	a5,0
    8000267a:	b7d9                	j	80002640 <printstr+0x1c>

000000008000267c <thread_entry>:
    8000267c:	e101                	bnez	a0,8000267c <thread_entry>
    8000267e:	8082                	ret

0000000080002680 <printhex>:
    80002680:	7179                	addi	sp,sp,-48
    80002682:	87aa                	mv	a5,a0
    80002684:	f406                	sd	ra,40(sp)
    80002686:	0028                	addi	a0,sp,8
    80002688:	01710613          	addi	a2,sp,23
    8000268c:	4825                	li	a6,9
    8000268e:	a011                	j	80002692 <printhex+0x12>
    80002690:	863a                	mv	a2,a4
    80002692:	00f7f693          	andi	a3,a5,15
    80002696:	8736                	mv	a4,a3
    80002698:	03000593          	li	a1,48
    8000269c:	00d87463          	bgeu	a6,a3,800026a4 <printhex+0x24>
    800026a0:	05700593          	li	a1,87
    800026a4:	9f2d                	addw	a4,a4,a1
    800026a6:	00e60023          	sb	a4,0(a2)
    800026aa:	8391                	srli	a5,a5,0x4
    800026ac:	fff60713          	addi	a4,a2,-1
    800026b0:	fec510e3          	bne	a0,a2,80002690 <printhex+0x10>
    800026b4:	00010c23          	sb	zero,24(sp)
    800026b8:	f6dff0ef          	jal	ra,80002624 <printstr>
    800026bc:	70a2                	ld	ra,40(sp)
    800026be:	6145                	addi	sp,sp,48
    800026c0:	8082                	ret

00000000800026c2 <printf>:
    800026c2:	711d                	addi	sp,sp,-96
    800026c4:	02810313          	addi	t1,sp,40
    800026c8:	8e2a                	mv	t3,a0
    800026ca:	f42e                	sd	a1,40(sp)
    800026cc:	f832                	sd	a2,48(sp)
    800026ce:	fc36                	sd	a3,56(sp)
    800026d0:	00000517          	auipc	a0,0x0
    800026d4:	e7050513          	addi	a0,a0,-400 # 80002540 <putchar>
    800026d8:	869a                	mv	a3,t1
    800026da:	8672                	mv	a2,t3
    800026dc:	4581                	li	a1,0
    800026de:	ec06                	sd	ra,24(sp)
    800026e0:	e0ba                	sd	a4,64(sp)
    800026e2:	e4be                	sd	a5,72(sp)
    800026e4:	e8c2                	sd	a6,80(sp)
    800026e6:	ecc6                	sd	a7,88(sp)
    800026e8:	e41a                	sd	t1,8(sp)
    800026ea:	b7bff0ef          	jal	ra,80002264 <vprintfmt>
    800026ee:	60e2                	ld	ra,24(sp)
    800026f0:	4501                	li	a0,0
    800026f2:	6125                	addi	sp,sp,96
    800026f4:	8082                	ret

00000000800026f6 <sprintf>:
    800026f6:	711d                	addi	sp,sp,-96
    800026f8:	03010313          	addi	t1,sp,48
    800026fc:	f022                	sd	s0,32(sp)
    800026fe:	e42a                	sd	a0,8(sp)
    80002700:	f832                	sd	a2,48(sp)
    80002702:	fc36                	sd	a3,56(sp)
    80002704:	842a                	mv	s0,a0
    80002706:	862e                	mv	a2,a1
    80002708:	00000517          	auipc	a0,0x0
    8000270c:	e2a50513          	addi	a0,a0,-470 # 80002532 <sprintf_putch.0>
    80002710:	002c                	addi	a1,sp,8
    80002712:	869a                	mv	a3,t1
    80002714:	f406                	sd	ra,40(sp)
    80002716:	e4be                	sd	a5,72(sp)
    80002718:	e0ba                	sd	a4,64(sp)
    8000271a:	e8c2                	sd	a6,80(sp)
    8000271c:	ecc6                	sd	a7,88(sp)
    8000271e:	ec1a                	sd	t1,24(sp)
    80002720:	b45ff0ef          	jal	ra,80002264 <vprintfmt>
    80002724:	67a2                	ld	a5,8(sp)
    80002726:	00078023          	sb	zero,0(a5)
    8000272a:	6522                	ld	a0,8(sp)
    8000272c:	70a2                	ld	ra,40(sp)
    8000272e:	9d01                	subw	a0,a0,s0
    80002730:	7402                	ld	s0,32(sp)
    80002732:	6125                	addi	sp,sp,96
    80002734:	8082                	ret

0000000080002736 <memcpy>:
    80002736:	00b567b3          	or	a5,a0,a1
    8000273a:	8fd1                	or	a5,a5,a2
    8000273c:	8b9d                	andi	a5,a5,7
    8000273e:	00c506b3          	add	a3,a0,a2
    80002742:	cf91                	beqz	a5,8000275e <memcpy+0x28>
    80002744:	962e                	add	a2,a2,a1
    80002746:	87aa                	mv	a5,a0
    80002748:	02d57663          	bgeu	a0,a3,80002774 <memcpy+0x3e>
    8000274c:	0005c703          	lbu	a4,0(a1)
    80002750:	0585                	addi	a1,a1,1
    80002752:	0785                	addi	a5,a5,1
    80002754:	fee78fa3          	sb	a4,-1(a5)
    80002758:	feb61ae3          	bne	a2,a1,8000274c <memcpy+0x16>
    8000275c:	8082                	ret
    8000275e:	fed57fe3          	bgeu	a0,a3,8000275c <memcpy+0x26>
    80002762:	87aa                	mv	a5,a0
    80002764:	6198                	ld	a4,0(a1)
    80002766:	07a1                	addi	a5,a5,8
    80002768:	05a1                	addi	a1,a1,8
    8000276a:	fee7bc23          	sd	a4,-8(a5)
    8000276e:	fed7ebe3          	bltu	a5,a3,80002764 <memcpy+0x2e>
    80002772:	8082                	ret
    80002774:	8082                	ret

0000000080002776 <memset>:
    80002776:	00c567b3          	or	a5,a0,a2
    8000277a:	8b9d                	andi	a5,a5,7
    8000277c:	962a                	add	a2,a2,a0
    8000277e:	0ff5f593          	andi	a1,a1,255
    80002782:	cb91                	beqz	a5,80002796 <memset+0x20>
    80002784:	87aa                	mv	a5,a0
    80002786:	02c57a63          	bgeu	a0,a2,800027ba <memset+0x44>
    8000278a:	0785                	addi	a5,a5,1
    8000278c:	feb78fa3          	sb	a1,-1(a5)
    80002790:	fec79de3          	bne	a5,a2,8000278a <memset+0x14>
    80002794:	8082                	ret
    80002796:	00859793          	slli	a5,a1,0x8
    8000279a:	8fcd                	or	a5,a5,a1
    8000279c:	01079713          	slli	a4,a5,0x10
    800027a0:	8fd9                	or	a5,a5,a4
    800027a2:	02079713          	slli	a4,a5,0x20
    800027a6:	8fd9                	or	a5,a5,a4
    800027a8:	fec576e3          	bgeu	a0,a2,80002794 <memset+0x1e>
    800027ac:	872a                	mv	a4,a0
    800027ae:	0721                	addi	a4,a4,8
    800027b0:	fef73c23          	sd	a5,-8(a4)
    800027b4:	fec76de3          	bltu	a4,a2,800027ae <memset+0x38>
    800027b8:	8082                	ret
    800027ba:	8082                	ret

00000000800027bc <_init>:
    800027bc:	7171                	addi	sp,sp,-176
    800027be:	00004797          	auipc	a5,0x4
    800027c2:	88a78793          	addi	a5,a5,-1910 # 80006048 <_tdata_begin>
    800027c6:	e152                	sd	s4,128(sp)
    800027c8:	00004a17          	auipc	s4,0x4
    800027cc:	880a0a13          	addi	s4,s4,-1920 # 80006048 <_tdata_begin>
    800027d0:	e54e                	sd	s3,136(sp)
    800027d2:	40fa09b3          	sub	s3,s4,a5
    800027d6:	ed26                	sd	s1,152(sp)
    800027d8:	e94a                	sd	s2,144(sp)
    800027da:	84aa                	mv	s1,a0
    800027dc:	892e                	mv	s2,a1
    800027de:	8512                	mv	a0,tp
    800027e0:	85be                	mv	a1,a5
    800027e2:	864e                	mv	a2,s3
    800027e4:	f506                	sd	ra,168(sp)
    800027e6:	f122                	sd	s0,160(sp)
    800027e8:	fcd6                	sd	s5,120(sp)
    800027ea:	8a92                	mv	s5,tp
    800027ec:	f4bff0ef          	jal	ra,80002736 <memcpy>
    800027f0:	00004617          	auipc	a2,0x4
    800027f4:	8d460613          	addi	a2,a2,-1836 # 800060c4 <_tbss_end>
    800027f8:	41460633          	sub	a2,a2,s4
    800027fc:	4581                	li	a1,0
    800027fe:	013a8533          	add	a0,s5,s3
    80002802:	f75ff0ef          	jal	ra,80002776 <memset>
    80002806:	85ca                	mv	a1,s2
    80002808:	8526                	mv	a0,s1
    8000280a:	e73ff0ef          	jal	ra,8000267c <thread_entry>
    8000280e:	4581                	li	a1,0
    80002810:	4501                	li	a0,0
    80002812:	146000ef          	jal	ra,80002958 <main>
    80002816:	00004917          	auipc	s2,0x4
    8000281a:	81290913          	addi	s2,s2,-2030 # 80006028 <counters>
    8000281e:	00093683          	ld	a3,0(s2)
    80002822:	03f10413          	addi	s0,sp,63
    80002826:	fc047413          	andi	s0,s0,-64
    8000282a:	84aa                	mv	s1,a0
    8000282c:	ea85                	bnez	a3,8000285c <_init+0xa0>
    8000282e:	00893683          	ld	a3,8(s2)
    80002832:	e681                	bnez	a3,8000283a <_init+0x7e>
    80002834:	8526                	mv	a0,s1
    80002836:	db7ff0ef          	jal	ra,800025ec <tohost_exit>
    8000283a:	89a2                	mv	s3,s0
    8000283c:	01893603          	ld	a2,24(s2)
    80002840:	854e                	mv	a0,s3
    80002842:	00001597          	auipc	a1,0x1
    80002846:	e5658593          	addi	a1,a1,-426 # 80003698 <main+0xd40>
    8000284a:	eadff0ef          	jal	ra,800026f6 <sprintf>
    8000284e:	99aa                	add	s3,s3,a0
    80002850:	ff3402e3          	beq	s0,s3,80002834 <_init+0x78>
    80002854:	8522                	mv	a0,s0
    80002856:	dcfff0ef          	jal	ra,80002624 <printstr>
    8000285a:	bfe9                	j	80002834 <_init+0x78>
    8000285c:	01093603          	ld	a2,16(s2)
    80002860:	00001597          	auipc	a1,0x1
    80002864:	e3858593          	addi	a1,a1,-456 # 80003698 <main+0xd40>
    80002868:	8522                	mv	a0,s0
    8000286a:	e8dff0ef          	jal	ra,800026f6 <sprintf>
    8000286e:	00893683          	ld	a3,8(s2)
    80002872:	00a409b3          	add	s3,s0,a0
    80002876:	dee9                	beqz	a3,80002850 <_init+0x94>
    80002878:	b7d1                	j	8000283c <_init+0x80>

000000008000287a <strlen>:
    8000287a:	00054783          	lbu	a5,0(a0)
    8000287e:	cb89                	beqz	a5,80002890 <strlen+0x16>
    80002880:	87aa                	mv	a5,a0
    80002882:	0017c703          	lbu	a4,1(a5)
    80002886:	0785                	addi	a5,a5,1
    80002888:	ff6d                	bnez	a4,80002882 <strlen+0x8>
    8000288a:	40a78533          	sub	a0,a5,a0
    8000288e:	8082                	ret
    80002890:	4501                	li	a0,0
    80002892:	8082                	ret

0000000080002894 <strnlen>:
    80002894:	00b506b3          	add	a3,a0,a1
    80002898:	87aa                	mv	a5,a0
    8000289a:	e589                	bnez	a1,800028a4 <strnlen+0x10>
    8000289c:	a829                	j	800028b6 <strnlen+0x22>
    8000289e:	0785                	addi	a5,a5,1
    800028a0:	00f68863          	beq	a3,a5,800028b0 <strnlen+0x1c>
    800028a4:	0007c703          	lbu	a4,0(a5)
    800028a8:	fb7d                	bnez	a4,8000289e <strnlen+0xa>
    800028aa:	40a78533          	sub	a0,a5,a0
    800028ae:	8082                	ret
    800028b0:	40a68533          	sub	a0,a3,a0
    800028b4:	8082                	ret
    800028b6:	4501                	li	a0,0
    800028b8:	8082                	ret

00000000800028ba <strcmp>:
    800028ba:	00054783          	lbu	a5,0(a0)
    800028be:	0585                	addi	a1,a1,1
    800028c0:	0505                	addi	a0,a0,1
    800028c2:	fff5c703          	lbu	a4,-1(a1)
    800028c6:	c799                	beqz	a5,800028d4 <strcmp+0x1a>
    800028c8:	fee789e3          	beq	a5,a4,800028ba <strcmp>
    800028cc:	0007851b          	sext.w	a0,a5
    800028d0:	9d19                	subw	a0,a0,a4
    800028d2:	8082                	ret
    800028d4:	4501                	li	a0,0
    800028d6:	bfed                	j	800028d0 <strcmp+0x16>

00000000800028d8 <strcpy>:
    800028d8:	87aa                	mv	a5,a0
    800028da:	0005c703          	lbu	a4,0(a1)
    800028de:	0785                	addi	a5,a5,1
    800028e0:	0585                	addi	a1,a1,1
    800028e2:	fee78fa3          	sb	a4,-1(a5)
    800028e6:	fb75                	bnez	a4,800028da <strcpy+0x2>
    800028e8:	8082                	ret

00000000800028ea <atol>:
    800028ea:	00054703          	lbu	a4,0(a0)
    800028ee:	02000693          	li	a3,32
    800028f2:	87aa                	mv	a5,a0
    800028f4:	00d71763          	bne	a4,a3,80002902 <atol+0x18>
    800028f8:	0017c703          	lbu	a4,1(a5)
    800028fc:	0785                	addi	a5,a5,1
    800028fe:	fed70de3          	beq	a4,a3,800028f8 <atol+0xe>
    80002902:	02d00693          	li	a3,45
    80002906:	04d70263          	beq	a4,a3,8000294a <atol+0x60>
    8000290a:	02b00693          	li	a3,43
    8000290e:	02d70763          	beq	a4,a3,8000293c <atol+0x52>
    80002912:	0007c683          	lbu	a3,0(a5)
    80002916:	4581                	li	a1,0
    80002918:	c69d                	beqz	a3,80002946 <atol+0x5c>
    8000291a:	4501                	li	a0,0
    8000291c:	0785                	addi	a5,a5,1
    8000291e:	00251713          	slli	a4,a0,0x2
    80002922:	fd06861b          	addiw	a2,a3,-48
    80002926:	0007c683          	lbu	a3,0(a5)
    8000292a:	972a                	add	a4,a4,a0
    8000292c:	0706                	slli	a4,a4,0x1
    8000292e:	00e60533          	add	a0,a2,a4
    80002932:	f6ed                	bnez	a3,8000291c <atol+0x32>
    80002934:	c991                	beqz	a1,80002948 <atol+0x5e>
    80002936:	40a00533          	neg	a0,a0
    8000293a:	8082                	ret
    8000293c:	0017c683          	lbu	a3,1(a5)
    80002940:	4581                	li	a1,0
    80002942:	0785                	addi	a5,a5,1
    80002944:	faf9                	bnez	a3,8000291a <atol+0x30>
    80002946:	4501                	li	a0,0
    80002948:	8082                	ret
    8000294a:	0017c683          	lbu	a3,1(a5)
    8000294e:	4585                	li	a1,1
    80002950:	0785                	addi	a5,a5,1
    80002952:	f6e1                	bnez	a3,8000291a <atol+0x30>
    80002954:	4501                	li	a0,0
    80002956:	bfcd                	j	80002948 <atol+0x5e>

Disassembly of section .text.startup:

0000000080002958 <main>:
    80002958:	7155                	addi	sp,sp,-208
    8000295a:	e586                	sd	ra,200(sp)
    8000295c:	e1a2                	sd	s0,192(sp)
    8000295e:	fd26                	sd	s1,184(sp)
    80002960:	0980                	addi	s0,sp,208
    80002962:	f94a                	sd	s2,176(sp)
    80002964:	f54e                	sd	s3,168(sp)
    80002966:	f152                	sd	s4,160(sp)
    80002968:	ed56                	sd	s5,152(sp)
    8000296a:	e95a                	sd	s6,144(sp)
    8000296c:	e55e                	sd	s7,136(sp)
    8000296e:	e162                	sd	s8,128(sp)
    80002970:	fce6                	sd	s9,120(sp)
    80002972:	f8ea                	sd	s10,112(sp)
    80002974:	f4ee                	sd	s11,104(sp)
    80002976:	00000797          	auipc	a5,0x0
    8000297a:	6ca78793          	addi	a5,a5,1738 # 80003040 <main+0x6e8>
    8000297e:	7139                	addi	sp,sp,-64
    80002980:	860a                	mv	a2,sp
    80002982:	0007a983          	lw	s3,0(a5)
    80002986:	7139                	addi	sp,sp,-64
    80002988:	4685                	li	a3,1
    8000298a:	0047af83          	lw	t6,4(a5)
    8000298e:	0087af03          	lw	t5,8(a5)
    80002992:	00c7ae83          	lw	t4,12(a5)
    80002996:	0107ae03          	lw	t3,16(a5)
    8000299a:	0147a303          	lw	t1,20(a5)
    8000299e:	0187a883          	lw	a7,24(a5)
    800029a2:	01c7d803          	lhu	a6,28(a5)
    800029a6:	01e7c503          	lbu	a0,30(a5)
    800029aa:	1686                	slli	a3,a3,0x21
    800029ac:	878a                	mv	a5,sp
    800029ae:	e794                	sd	a3,8(a5)
    800029b0:	02800693          	li	a3,40
    800029b4:	00001717          	auipc	a4,0x1
    800029b8:	c4c70713          	addi	a4,a4,-948 # 80003600 <main+0xca8>
    800029bc:	e390                	sd	a2,0(a5)
    800029be:	cb94                	sw	a3,16(a5)
    800029c0:	0137aa23          	sw	s3,20(a5)
    800029c4:	00001697          	auipc	a3,0x1
    800029c8:	e7c68693          	addi	a3,a3,-388 # 80003840 <Next_Ptr_Glob>
    800029cc:	01073383          	ld	t2,16(a4)
    800029d0:	01872283          	lw	t0,24(a4)
    800029d4:	01c75583          	lhu	a1,28(a4)
    800029d8:	00073903          	ld	s2,0(a4)
    800029dc:	6704                	ld	s1,8(a4)
    800029de:	e290                	sd	a2,0(a3)
    800029e0:	84c18693          	addi	a3,gp,-1972 # 80003848 <Ptr_Glob>
    800029e4:	e29c                	sd	a5,0(a3)
    800029e6:	01f7ac23          	sw	t6,24(a5)
    800029ea:	01e7ae23          	sw	t5,28(a5)
    800029ee:	03d7a023          	sw	t4,32(a5)
    800029f2:	03c7a223          	sw	t3,36(a5)
    800029f6:	0267a423          	sw	t1,40(a5)
    800029fa:	0317a623          	sw	a7,44(a5)
    800029fe:	03079823          	sh	a6,48(a5)
    80002a02:	02a78923          	sb	a0,50(a5)
    80002a06:	01e74783          	lbu	a5,30(a4)
    80002a0a:	00001717          	auipc	a4,0x1
    80002a0e:	f0e70713          	addi	a4,a4,-242 # 80003918 <Arr_2_Glob>
    80002a12:	00001517          	auipc	a0,0x1
    80002a16:	8ce50513          	addi	a0,a0,-1842 # 800032e0 <main+0x988>
    80002a1a:	f6f40723          	sb	a5,-146(s0)
    80002a1e:	47a9                	li	a5,10
    80002a20:	64f72e23          	sw	a5,1628(a4)
    80002a24:	f6743023          	sd	t2,-160(s0)
    80002a28:	f6542423          	sw	t0,-152(s0)
    80002a2c:	f6b41623          	sh	a1,-148(s0)
    80002a30:	f5243823          	sd	s2,-176(s0)
    80002a34:	f4943c23          	sd	s1,-168(s0)
    80002a38:	ef0ff0ef          	jal	ra,80002128 <debug_printf>
    80002a3c:	00000597          	auipc	a1,0x0
    80002a40:	62458593          	addi	a1,a1,1572 # 80003060 <main+0x708>
    80002a44:	00000517          	auipc	a0,0x0
    80002a48:	62c50513          	addi	a0,a0,1580 # 80003070 <main+0x718>
    80002a4c:	edcff0ef          	jal	ra,80002128 <debug_printf>
    80002a50:	00001797          	auipc	a5,0x1
    80002a54:	ddc7a783          	lw	a5,-548(a5) # 8000382c <Reg>
    80002a58:	5c078163          	beqz	a5,8000301a <main+0x6c2>
    80002a5c:	00000517          	auipc	a0,0x0
    80002a60:	63c50513          	addi	a0,a0,1596 # 80003098 <main+0x740>
    80002a64:	ec4ff0ef          	jal	ra,80002128 <debug_printf>
    80002a68:	000f4637          	lui	a2,0xf4
    80002a6c:	24060613          	addi	a2,a2,576 # f4240 <buflen.1+0xf4200>
    80002a70:	00000597          	auipc	a1,0x0
    80002a74:	68858593          	addi	a1,a1,1672 # 800030f8 <main+0x7a0>
    80002a78:	00000517          	auipc	a0,0x0
    80002a7c:	69050513          	addi	a0,a0,1680 # 80003108 <main+0x7b0>
    80002a80:	ea8ff0ef          	jal	ra,80002128 <debug_printf>
    80002a84:	00001517          	auipc	a0,0x1
    80002a88:	85c50513          	addi	a0,a0,-1956 # 800032e0 <main+0x988>
    80002a8c:	e9cff0ef          	jal	ra,80002128 <debug_printf>
    80002a90:	1f400793          	li	a5,500
    80002a94:	f2f43823          	sd	a5,-208(s0)
    80002a98:	00001a17          	auipc	s4,0x1
    80002a9c:	b88a0a13          	addi	s4,s4,-1144 # 80003620 <main+0xcc8>
    80002aa0:	00001797          	auipc	a5,0x1
    80002aa4:	d807a423          	sw	zero,-632(a5) # 80003828 <Done>
    80002aa8:	00001917          	auipc	s2,0x1
    80002aac:	d8890913          	addi	s2,s2,-632 # 80003830 <Ch_2_Glob>
    80002ab0:	00001b97          	auipc	s7,0x1
    80002ab4:	d88b8b93          	addi	s7,s7,-632 # 80003838 <Int_Glob>
    80002ab8:	00001d97          	auipc	s11,0x1
    80002abc:	e60d8d93          	addi	s11,s11,-416 # 80003918 <Arr_2_Glob>
    80002ac0:	85418d13          	addi	s10,gp,-1964 # 80003850 <Arr_1_Glob>
    80002ac4:	00001497          	auipc	s1,0x1
    80002ac8:	b7c48493          	addi	s1,s1,-1156 # 80003640 <main+0xce8>
    80002acc:	f3043a83          	ld	s5,-208(s0)
    80002ad0:	00000517          	auipc	a0,0x0
    80002ad4:	65050513          	addi	a0,a0,1616 # 80003120 <main+0x7c8>
    80002ad8:	85d6                	mv	a1,s5
    80002ada:	e4eff0ef          	jal	ra,80002128 <debug_printf>
    80002ade:	4505                	li	a0,1
    80002ae0:	ad7ff0ef          	jal	ra,800025b6 <setStats>
    80002ae4:	b00027f3          	csrr	a5,mcycle
    80002ae8:	00001717          	auipc	a4,0x1
    80002aec:	d3870713          	addi	a4,a4,-712 # 80003820 <Begin_Time>
    80002af0:	4985                	li	s3,1
    80002af2:	e31c                	sd	a5,0(a4)
    80002af4:	001a8c9b          	addiw	s9,s5,1
    80002af8:	4c05                	li	s8,1
    80002afa:	f54ff0ef          	jal	ra,8000224e <Proc_5>
    80002afe:	f24ff0ef          	jal	ra,80002222 <Proc_4>
    80002b02:	000a3883          	ld	a7,0(s4)
    80002b06:	008a3803          	ld	a6,8(s4)
    80002b0a:	018a2683          	lw	a3,24(s4)
    80002b0e:	010a3603          	ld	a2,16(s4)
    80002b12:	01ca5703          	lhu	a4,28(s4)
    80002b16:	01ea4783          	lbu	a5,30(s4)
    80002b1a:	f7040593          	addi	a1,s0,-144
    80002b1e:	f5040513          	addi	a0,s0,-176
    80002b22:	f7143823          	sd	a7,-144(s0)
    80002b26:	f7043c23          	sd	a6,-136(s0)
    80002b2a:	f8d42423          	sw	a3,-120(s0)
    80002b2e:	f8c43023          	sd	a2,-128(s0)
    80002b32:	f8e41623          	sh	a4,-116(s0)
    80002b36:	f8f40723          	sb	a5,-114(s0)
    80002b3a:	f5842623          	sw	s8,-180(s0)
    80002b3e:	d30ff0ef          	jal	ra,8000206e <Func_2>
    80002b42:	00001717          	auipc	a4,0x1
    80002b46:	cf270713          	addi	a4,a4,-782 # 80003834 <Bool_Glob>
    80002b4a:	00153793          	seqz	a5,a0
    80002b4e:	c31c                	sw	a5,0(a4)
    80002b50:	f4840613          	addi	a2,s0,-184
    80002b54:	479d                	li	a5,7
    80002b56:	458d                	li	a1,3
    80002b58:	4509                	li	a0,2
    80002b5a:	f4f42423          	sw	a5,-184(s0)
    80002b5e:	ca2ff0ef          	jal	ra,80002000 <Proc_7>
    80002b62:	f4842683          	lw	a3,-184(s0)
    80002b66:	478d                	li	a5,3
    80002b68:	460d                	li	a2,3
    80002b6a:	85ee                	mv	a1,s11
    80002b6c:	856a                	mv	a0,s10
    80002b6e:	f4f42223          	sw	a5,-188(s0)
    80002b72:	c96ff0ef          	jal	ra,80002008 <Proc_8>
    80002b76:	84c18793          	addi	a5,gp,-1972 # 80003848 <Ptr_Glob>
    80002b7a:	6388                	ld	a0,0(a5)
    80002b7c:	e00ff0ef          	jal	ra,8000217c <Proc_1>
    80002b80:	00094703          	lbu	a4,0(s2)
    80002b84:	04000793          	li	a5,64
    80002b88:	44e7fa63          	bgeu	a5,a4,80002fdc <main+0x684>
    80002b8c:	04100a93          	li	s5,65
    80002b90:	4b0d                	li	s6,3
    80002b92:	a801                	j	80002ba2 <main+0x24a>
    80002b94:	00094783          	lbu	a5,0(s2)
    80002b98:	2a85                	addiw	s5,s5,1
    80002b9a:	0ffafa93          	andi	s5,s5,255
    80002b9e:	0557ef63          	bltu	a5,s5,80002bfc <main+0x2a4>
    80002ba2:	04300593          	li	a1,67
    80002ba6:	8556                	mv	a0,s5
    80002ba8:	caaff0ef          	jal	ra,80002052 <Func_1>
    80002bac:	f4c42783          	lw	a5,-180(s0)
    80002bb0:	2501                	sext.w	a0,a0
    80002bb2:	fea791e3          	bne	a5,a0,80002b94 <main+0x23c>
    80002bb6:	f4c40593          	addi	a1,s0,-180
    80002bba:	4501                	li	a0,0
    80002bbc:	cfcff0ef          	jal	ra,800020b8 <Proc_6>
    80002bc0:	01e4c783          	lbu	a5,30(s1)
    80002bc4:	6088                	ld	a0,0(s1)
    80002bc6:	648c                	ld	a1,8(s1)
    80002bc8:	6890                	ld	a2,16(s1)
    80002bca:	4c94                	lw	a3,24(s1)
    80002bcc:	01c4d703          	lhu	a4,28(s1)
    80002bd0:	f8f40723          	sb	a5,-114(s0)
    80002bd4:	00094783          	lbu	a5,0(s2)
    80002bd8:	2a85                	addiw	s5,s5,1
    80002bda:	f6a43823          	sd	a0,-144(s0)
    80002bde:	f6b43c23          	sd	a1,-136(s0)
    80002be2:	f8c43023          	sd	a2,-128(s0)
    80002be6:	f8d42423          	sw	a3,-120(s0)
    80002bea:	f8e41623          	sh	a4,-116(s0)
    80002bee:	013ba023          	sw	s3,0(s7)
    80002bf2:	0ffafa93          	andi	s5,s5,255
    80002bf6:	8b4e                	mv	s6,s3
    80002bf8:	fb57f5e3          	bgeu	a5,s5,80002ba2 <main+0x24a>
    80002bfc:	001b1a9b          	slliw	s5,s6,0x1
    80002c00:	016a8abb          	addw	s5,s5,s6
    80002c04:	f4842b03          	lw	s6,-184(s0)
    80002c08:	2985                	addiw	s3,s3,1
    80002c0a:	f4440513          	addi	a0,s0,-188
    80002c0e:	036ac73b          	divw	a4,s5,s6
    80002c12:	f2e43c23          	sd	a4,-200(s0)
    80002c16:	f4e42223          	sw	a4,-188(s0)
    80002c1a:	d22ff0ef          	jal	ra,8000213c <Proc_2>
    80002c1e:	ed3c9ee3          	bne	s9,s3,80002afa <main+0x1a2>
    80002c22:	b00027f3          	csrr	a5,mcycle
    80002c26:	00001717          	auipc	a4,0x1
    80002c2a:	bf270713          	addi	a4,a4,-1038 # 80003818 <End_Time>
    80002c2e:	4501                	li	a0,0
    80002c30:	89ba                	mv	s3,a4
    80002c32:	e31c                	sd	a5,0(a4)
    80002c34:	983ff0ef          	jal	ra,800025b6 <setStats>
    80002c38:	00001717          	auipc	a4,0x1
    80002c3c:	be870713          	addi	a4,a4,-1048 # 80003820 <Begin_Time>
    80002c40:	6318                	ld	a4,0(a4)
    80002c42:	0009b783          	ld	a5,0(s3)
    80002c46:	8f99                	sub	a5,a5,a4
    80002c48:	00001717          	auipc	a4,0x1
    80002c4c:	bc870713          	addi	a4,a4,-1080 # 80003810 <User_Time>
    80002c50:	e31c                	sd	a5,0(a4)
    80002c52:	38f05763          	blez	a5,80002fe0 <main+0x688>
    80002c56:	4785                	li	a5,1
    80002c58:	00001717          	auipc	a4,0x1
    80002c5c:	bd070713          	addi	a4,a4,-1072 # 80003828 <Done>
    80002c60:	c31c                	sw	a5,0(a4)
    80002c62:	00000517          	auipc	a0,0x0
    80002c66:	51e50513          	addi	a0,a0,1310 # 80003180 <main+0x828>
    80002c6a:	cbeff0ef          	jal	ra,80002128 <debug_printf>
    80002c6e:	00000517          	auipc	a0,0x0
    80002c72:	67250513          	addi	a0,a0,1650 # 800032e0 <main+0x988>
    80002c76:	cb2ff0ef          	jal	ra,80002128 <debug_printf>
    80002c7a:	000ba583          	lw	a1,0(s7)
    80002c7e:	00000517          	auipc	a0,0x0
    80002c82:	53a50513          	addi	a0,a0,1338 # 800031b8 <main+0x860>
    80002c86:	416a8abb          	subw	s5,s5,s6
    80002c8a:	c9eff0ef          	jal	ra,80002128 <debug_printf>
    80002c8e:	4595                	li	a1,5
    80002c90:	00000517          	auipc	a0,0x0
    80002c94:	54850513          	addi	a0,a0,1352 # 800031d8 <main+0x880>
    80002c98:	c90ff0ef          	jal	ra,80002128 <debug_printf>
    80002c9c:	00001797          	auipc	a5,0x1
    80002ca0:	b9878793          	addi	a5,a5,-1128 # 80003834 <Bool_Glob>
    80002ca4:	438c                	lw	a1,0(a5)
    80002ca6:	00000517          	auipc	a0,0x0
    80002caa:	55250513          	addi	a0,a0,1362 # 800031f8 <main+0x8a0>
    80002cae:	00001497          	auipc	s1,0x1
    80002cb2:	b5248493          	addi	s1,s1,-1198 # 80003800 <Dhrystones_Per_Second>
    80002cb6:	c72ff0ef          	jal	ra,80002128 <debug_printf>
    80002cba:	4585                	li	a1,1
    80002cbc:	00000517          	auipc	a0,0x0
    80002cc0:	51c50513          	addi	a0,a0,1308 # 800031d8 <main+0x880>
    80002cc4:	c64ff0ef          	jal	ra,80002128 <debug_printf>
    80002cc8:	00001597          	auipc	a1,0x1
    80002ccc:	b695c583          	lbu	a1,-1175(a1) # 80003831 <Ch_1_Glob>
    80002cd0:	00000517          	auipc	a0,0x0
    80002cd4:	54850513          	addi	a0,a0,1352 # 80003218 <main+0x8c0>
    80002cd8:	c50ff0ef          	jal	ra,80002128 <debug_printf>
    80002cdc:	04100593          	li	a1,65
    80002ce0:	00000517          	auipc	a0,0x0
    80002ce4:	55850513          	addi	a0,a0,1368 # 80003238 <main+0x8e0>
    80002ce8:	c40ff0ef          	jal	ra,80002128 <debug_printf>
    80002cec:	00094583          	lbu	a1,0(s2)
    80002cf0:	00000517          	auipc	a0,0x0
    80002cf4:	56850513          	addi	a0,a0,1384 # 80003258 <main+0x900>
    80002cf8:	84c18913          	addi	s2,gp,-1972 # 80003848 <Ptr_Glob>
    80002cfc:	c2cff0ef          	jal	ra,80002128 <debug_printf>
    80002d00:	04200593          	li	a1,66
    80002d04:	00000517          	auipc	a0,0x0
    80002d08:	53450513          	addi	a0,a0,1332 # 80003238 <main+0x8e0>
    80002d0c:	c1cff0ef          	jal	ra,80002128 <debug_printf>
    80002d10:	8741a583          	lw	a1,-1932(gp) # 80003870 <Arr_1_Glob+0x20>
    80002d14:	00000517          	auipc	a0,0x0
    80002d18:	56450513          	addi	a0,a0,1380 # 80003278 <main+0x920>
    80002d1c:	c0cff0ef          	jal	ra,80002128 <debug_printf>
    80002d20:	459d                	li	a1,7
    80002d22:	00000517          	auipc	a0,0x0
    80002d26:	4b650513          	addi	a0,a0,1206 # 800031d8 <main+0x880>
    80002d2a:	bfeff0ef          	jal	ra,80002128 <debug_printf>
    80002d2e:	00001797          	auipc	a5,0x1
    80002d32:	bea78793          	addi	a5,a5,-1046 # 80003918 <Arr_2_Glob>
    80002d36:	65c7a583          	lw	a1,1628(a5)
    80002d3a:	00000517          	auipc	a0,0x0
    80002d3e:	55e50513          	addi	a0,a0,1374 # 80003298 <main+0x940>
    80002d42:	be6ff0ef          	jal	ra,80002128 <debug_printf>
    80002d46:	00000517          	auipc	a0,0x0
    80002d4a:	57250513          	addi	a0,a0,1394 # 800032b8 <main+0x960>
    80002d4e:	bdaff0ef          	jal	ra,80002128 <debug_printf>
    80002d52:	00000517          	auipc	a0,0x0
    80002d56:	59650513          	addi	a0,a0,1430 # 800032e8 <main+0x990>
    80002d5a:	bceff0ef          	jal	ra,80002128 <debug_printf>
    80002d5e:	00093783          	ld	a5,0(s2)
    80002d62:	00000517          	auipc	a0,0x0
    80002d66:	59650513          	addi	a0,a0,1430 # 800032f8 <main+0x9a0>
    80002d6a:	638c                	ld	a1,0(a5)
    80002d6c:	bbcff0ef          	jal	ra,80002128 <debug_printf>
    80002d70:	00000517          	auipc	a0,0x0
    80002d74:	5a850513          	addi	a0,a0,1448 # 80003318 <main+0x9c0>
    80002d78:	bb0ff0ef          	jal	ra,80002128 <debug_printf>
    80002d7c:	00093783          	ld	a5,0(s2)
    80002d80:	00000517          	auipc	a0,0x0
    80002d84:	5d050513          	addi	a0,a0,1488 # 80003350 <main+0x9f8>
    80002d88:	478c                	lw	a1,8(a5)
    80002d8a:	b9eff0ef          	jal	ra,80002128 <debug_printf>
    80002d8e:	4581                	li	a1,0
    80002d90:	00000517          	auipc	a0,0x0
    80002d94:	44850513          	addi	a0,a0,1096 # 800031d8 <main+0x880>
    80002d98:	b90ff0ef          	jal	ra,80002128 <debug_printf>
    80002d9c:	00093783          	ld	a5,0(s2)
    80002da0:	00000517          	auipc	a0,0x0
    80002da4:	5d050513          	addi	a0,a0,1488 # 80003370 <main+0xa18>
    80002da8:	47cc                	lw	a1,12(a5)
    80002daa:	b7eff0ef          	jal	ra,80002128 <debug_printf>
    80002dae:	4589                	li	a1,2
    80002db0:	00000517          	auipc	a0,0x0
    80002db4:	42850513          	addi	a0,a0,1064 # 800031d8 <main+0x880>
    80002db8:	b70ff0ef          	jal	ra,80002128 <debug_printf>
    80002dbc:	00093783          	ld	a5,0(s2)
    80002dc0:	00000517          	auipc	a0,0x0
    80002dc4:	5d050513          	addi	a0,a0,1488 # 80003390 <main+0xa38>
    80002dc8:	4b8c                	lw	a1,16(a5)
    80002dca:	b5eff0ef          	jal	ra,80002128 <debug_printf>
    80002dce:	45c5                	li	a1,17
    80002dd0:	00000517          	auipc	a0,0x0
    80002dd4:	40850513          	addi	a0,a0,1032 # 800031d8 <main+0x880>
    80002dd8:	b50ff0ef          	jal	ra,80002128 <debug_printf>
    80002ddc:	00093583          	ld	a1,0(s2)
    80002de0:	00000517          	auipc	a0,0x0
    80002de4:	5d050513          	addi	a0,a0,1488 # 800033b0 <main+0xa58>
    80002de8:	00001917          	auipc	s2,0x1
    80002dec:	a5890913          	addi	s2,s2,-1448 # 80003840 <Next_Ptr_Glob>
    80002df0:	05d1                	addi	a1,a1,20
    80002df2:	b36ff0ef          	jal	ra,80002128 <debug_printf>
    80002df6:	00000517          	auipc	a0,0x0
    80002dfa:	5da50513          	addi	a0,a0,1498 # 800033d0 <main+0xa78>
    80002dfe:	b2aff0ef          	jal	ra,80002128 <debug_printf>
    80002e02:	00000517          	auipc	a0,0x0
    80002e06:	60650513          	addi	a0,a0,1542 # 80003408 <main+0xab0>
    80002e0a:	b1eff0ef          	jal	ra,80002128 <debug_printf>
    80002e0e:	00093783          	ld	a5,0(s2)
    80002e12:	00000517          	auipc	a0,0x0
    80002e16:	4e650513          	addi	a0,a0,1254 # 800032f8 <main+0x9a0>
    80002e1a:	638c                	ld	a1,0(a5)
    80002e1c:	b0cff0ef          	jal	ra,80002128 <debug_printf>
    80002e20:	00000517          	auipc	a0,0x0
    80002e24:	60050513          	addi	a0,a0,1536 # 80003420 <main+0xac8>
    80002e28:	b00ff0ef          	jal	ra,80002128 <debug_printf>
    80002e2c:	00093783          	ld	a5,0(s2)
    80002e30:	00000517          	auipc	a0,0x0
    80002e34:	52050513          	addi	a0,a0,1312 # 80003350 <main+0x9f8>
    80002e38:	478c                	lw	a1,8(a5)
    80002e3a:	aeeff0ef          	jal	ra,80002128 <debug_printf>
    80002e3e:	4581                	li	a1,0
    80002e40:	00000517          	auipc	a0,0x0
    80002e44:	39850513          	addi	a0,a0,920 # 800031d8 <main+0x880>
    80002e48:	ae0ff0ef          	jal	ra,80002128 <debug_printf>
    80002e4c:	00093783          	ld	a5,0(s2)
    80002e50:	00000517          	auipc	a0,0x0
    80002e54:	52050513          	addi	a0,a0,1312 # 80003370 <main+0xa18>
    80002e58:	47cc                	lw	a1,12(a5)
    80002e5a:	aceff0ef          	jal	ra,80002128 <debug_printf>
    80002e5e:	4585                	li	a1,1
    80002e60:	00000517          	auipc	a0,0x0
    80002e64:	37850513          	addi	a0,a0,888 # 800031d8 <main+0x880>
    80002e68:	ac0ff0ef          	jal	ra,80002128 <debug_printf>
    80002e6c:	00093783          	ld	a5,0(s2)
    80002e70:	00000517          	auipc	a0,0x0
    80002e74:	52050513          	addi	a0,a0,1312 # 80003390 <main+0xa38>
    80002e78:	4b8c                	lw	a1,16(a5)
    80002e7a:	aaeff0ef          	jal	ra,80002128 <debug_printf>
    80002e7e:	45c9                	li	a1,18
    80002e80:	00000517          	auipc	a0,0x0
    80002e84:	35850513          	addi	a0,a0,856 # 800031d8 <main+0x880>
    80002e88:	aa0ff0ef          	jal	ra,80002128 <debug_printf>
    80002e8c:	00093583          	ld	a1,0(s2)
    80002e90:	00000517          	auipc	a0,0x0
    80002e94:	52050513          	addi	a0,a0,1312 # 800033b0 <main+0xa58>
    80002e98:	05d1                	addi	a1,a1,20
    80002e9a:	a8eff0ef          	jal	ra,80002128 <debug_printf>
    80002e9e:	00000517          	auipc	a0,0x0
    80002ea2:	53250513          	addi	a0,a0,1330 # 800033d0 <main+0xa78>
    80002ea6:	a82ff0ef          	jal	ra,80002128 <debug_printf>
    80002eaa:	f4442583          	lw	a1,-188(s0)
    80002eae:	00000517          	auipc	a0,0x0
    80002eb2:	5b250513          	addi	a0,a0,1458 # 80003460 <main+0xb08>
    80002eb6:	a72ff0ef          	jal	ra,80002128 <debug_printf>
    80002eba:	4595                	li	a1,5
    80002ebc:	00000517          	auipc	a0,0x0
    80002ec0:	31c50513          	addi	a0,a0,796 # 800031d8 <main+0x880>
    80002ec4:	a64ff0ef          	jal	ra,80002128 <debug_printf>
    80002ec8:	f3843783          	ld	a5,-200(s0)
    80002ecc:	003a959b          	slliw	a1,s5,0x3
    80002ed0:	415585bb          	subw	a1,a1,s5
    80002ed4:	9d9d                	subw	a1,a1,a5
    80002ed6:	00000517          	auipc	a0,0x0
    80002eda:	5aa50513          	addi	a0,a0,1450 # 80003480 <main+0xb28>
    80002ede:	a4aff0ef          	jal	ra,80002128 <debug_printf>
    80002ee2:	45b5                	li	a1,13
    80002ee4:	00000517          	auipc	a0,0x0
    80002ee8:	2f450513          	addi	a0,a0,756 # 800031d8 <main+0x880>
    80002eec:	a3cff0ef          	jal	ra,80002128 <debug_printf>
    80002ef0:	f4842583          	lw	a1,-184(s0)
    80002ef4:	00000517          	auipc	a0,0x0
    80002ef8:	5ac50513          	addi	a0,a0,1452 # 800034a0 <main+0xb48>
    80002efc:	a2cff0ef          	jal	ra,80002128 <debug_printf>
    80002f00:	459d                	li	a1,7
    80002f02:	00000517          	auipc	a0,0x0
    80002f06:	2d650513          	addi	a0,a0,726 # 800031d8 <main+0x880>
    80002f0a:	a1eff0ef          	jal	ra,80002128 <debug_printf>
    80002f0e:	f4c42583          	lw	a1,-180(s0)
    80002f12:	00000517          	auipc	a0,0x0
    80002f16:	5ae50513          	addi	a0,a0,1454 # 800034c0 <main+0xb68>
    80002f1a:	a0eff0ef          	jal	ra,80002128 <debug_printf>
    80002f1e:	00000517          	auipc	a0,0x0
    80002f22:	2ba50513          	addi	a0,a0,698 # 800031d8 <main+0x880>
    80002f26:	4585                	li	a1,1
    80002f28:	a00ff0ef          	jal	ra,80002128 <debug_printf>
    80002f2c:	f5040593          	addi	a1,s0,-176
    80002f30:	00000517          	auipc	a0,0x0
    80002f34:	5b050513          	addi	a0,a0,1456 # 800034e0 <main+0xb88>
    80002f38:	9f0ff0ef          	jal	ra,80002128 <debug_printf>
    80002f3c:	00000517          	auipc	a0,0x0
    80002f40:	5c450513          	addi	a0,a0,1476 # 80003500 <main+0xba8>
    80002f44:	9e4ff0ef          	jal	ra,80002128 <debug_printf>
    80002f48:	f7040593          	addi	a1,s0,-144
    80002f4c:	00000517          	auipc	a0,0x0
    80002f50:	5ec50513          	addi	a0,a0,1516 # 80003538 <main+0xbe0>
    80002f54:	9d4ff0ef          	jal	ra,80002128 <debug_printf>
    80002f58:	00000517          	auipc	a0,0x0
    80002f5c:	60050513          	addi	a0,a0,1536 # 80003558 <main+0xc00>
    80002f60:	9c8ff0ef          	jal	ra,80002128 <debug_printf>
    80002f64:	00000517          	auipc	a0,0x0
    80002f68:	37c50513          	addi	a0,a0,892 # 800032e0 <main+0x988>
    80002f6c:	9bcff0ef          	jal	ra,80002128 <debug_printf>
    80002f70:	f3043783          	ld	a5,-208(s0)
    80002f74:	000f4737          	lui	a4,0xf4
    80002f78:	2407071b          	addiw	a4,a4,576
    80002f7c:	02f7073b          	mulw	a4,a4,a5
    80002f80:	00001697          	auipc	a3,0x1
    80002f84:	89068693          	addi	a3,a3,-1904 # 80003810 <User_Time>
    80002f88:	6294                	ld	a3,0(a3)
    80002f8a:	00000517          	auipc	a0,0x0
    80002f8e:	60650513          	addi	a0,a0,1542 # 80003590 <main+0xc38>
    80002f92:	02f6c7b3          	div	a5,a3,a5
    80002f96:	02d74733          	div	a4,a4,a3
    80002f9a:	85be                	mv	a1,a5
    80002f9c:	00001697          	auipc	a3,0x1
    80002fa0:	86f6b623          	sd	a5,-1940(a3) # 80003808 <Microseconds>
    80002fa4:	e098                	sd	a4,0(s1)
    80002fa6:	f1cff0ef          	jal	ra,800026c2 <printf>
    80002faa:	608c                	ld	a1,0(s1)
    80002fac:	00000517          	auipc	a0,0x0
    80002fb0:	61c50513          	addi	a0,a0,1564 # 800035c8 <main+0xc70>
    80002fb4:	f0eff0ef          	jal	ra,800026c2 <printf>
    80002fb8:	f3040113          	addi	sp,s0,-208
    80002fbc:	60ae                	ld	ra,200(sp)
    80002fbe:	640e                	ld	s0,192(sp)
    80002fc0:	74ea                	ld	s1,184(sp)
    80002fc2:	794a                	ld	s2,176(sp)
    80002fc4:	79aa                	ld	s3,168(sp)
    80002fc6:	7a0a                	ld	s4,160(sp)
    80002fc8:	6aea                	ld	s5,152(sp)
    80002fca:	6b4a                	ld	s6,144(sp)
    80002fcc:	6baa                	ld	s7,136(sp)
    80002fce:	6c0a                	ld	s8,128(sp)
    80002fd0:	7ce6                	ld	s9,120(sp)
    80002fd2:	7d46                	ld	s10,112(sp)
    80002fd4:	7da6                	ld	s11,104(sp)
    80002fd6:	4501                	li	a0,0
    80002fd8:	6169                	addi	sp,sp,208
    80002fda:	8082                	ret
    80002fdc:	4aa5                	li	s5,9
    80002fde:	b11d                	j	80002c04 <main+0x2ac>
    80002fe0:	00000517          	auipc	a0,0x0
    80002fe4:	16850513          	addi	a0,a0,360 # 80003148 <main+0x7f0>
    80002fe8:	edaff0ef          	jal	ra,800026c2 <printf>
    80002fec:	f3043703          	ld	a4,-208(s0)
    80002ff0:	00000517          	auipc	a0,0x0
    80002ff4:	2f050513          	addi	a0,a0,752 # 800032e0 <main+0x988>
    80002ff8:	0027179b          	slliw	a5,a4,0x2
    80002ffc:	9fb9                	addw	a5,a5,a4
    80002ffe:	0017979b          	slliw	a5,a5,0x1
    80003002:	f2f43823          	sd	a5,-208(s0)
    80003006:	ebcff0ef          	jal	ra,800026c2 <printf>
    8000300a:	00001797          	auipc	a5,0x1
    8000300e:	81e78793          	addi	a5,a5,-2018 # 80003828 <Done>
    80003012:	439c                	lw	a5,0(a5)
    80003014:	aa078ce3          	beqz	a5,80002acc <main+0x174>
    80003018:	b1a9                	j	80002c62 <main+0x30a>
    8000301a:	00000517          	auipc	a0,0x0
    8000301e:	0ae50513          	addi	a0,a0,174 # 800030c8 <main+0x770>
    80003022:	906ff0ef          	jal	ra,80002128 <debug_printf>
    80003026:	b489                	j	80002a68 <main+0x110>
    80003028:	1141                	addi	sp,sp,-16
    8000302a:	00000517          	auipc	a0,0x0
    8000302e:	65650513          	addi	a0,a0,1622 # 80003680 <main+0xd28>
    80003032:	e406                	sd	ra,8(sp)
    80003034:	df0ff0ef          	jal	ra,80002624 <printstr>
    80003038:	60a2                	ld	ra,8(sp)
    8000303a:	557d                	li	a0,-1
    8000303c:	0141                	addi	sp,sp,16
    8000303e:	8082                	ret

Disassembly of section .rodata.str1.8:

0000000080003040 <.rodata.str1.8>:
    80003040:	4844                	lw	s1,20(s0)
    80003042:	5952                	lw	s2,52(sp)
    80003044:	4e4f5453          	0x4e4f5453
    80003048:	2045                	0x2045
    8000304a:	5250                	lw	a2,36(a2)
    8000304c:	4152474f          	fnmadd.s	fa4,ft4,fs5,fs0,rmm
    80003050:	2c4d                	addiw	s8,s8,19
    80003052:	5320                	lw	s0,96(a4)
    80003054:	20454d4f          	fnmadd.s	fs10,fa0,ft4,ft4,rmm
    80003058:	49525453          	0x49525453
    8000305c:	474e                	lw	a4,208(sp)
    8000305e:	0000                	unimp
    80003060:	56202c43          	fmadd.q	fs8,ft0,ft2,fa0,rdn
    80003064:	7265                	lui	tp,0xffff9
    80003066:	6e6f6973          	csrrsi	s2,0x6e6,30
    8000306a:	3220                	fld	fs0,96(a2)
    8000306c:	322e                	fld	ft4,232(sp)
    8000306e:	0000                	unimp
    80003070:	6844                	ld	s1,144(s0)
    80003072:	7972                	ld	s2,312(sp)
    80003074:	6e6f7473          	csrrci	s0,0x6e6,30
    80003078:	2065                	0x2065
    8000307a:	6542                	ld	a0,16(sp)
    8000307c:	636e                	ld	t1,216(sp)
    8000307e:	6d68                	ld	a0,216(a0)
    80003080:	7261                	lui	tp,0xffff8
    80003082:	56202c6b          	0x56202c6b
    80003086:	7265                	lui	tp,0xffff9
    80003088:	6e6f6973          	csrrsi	s2,0x6e6,30
    8000308c:	2520                	fld	fs0,72(a0)
    8000308e:	00000a73          	0xa73
    80003092:	0000                	unimp
    80003094:	0000                	unimp
    80003096:	0000                	unimp
    80003098:	7250                	ld	a2,160(a2)
    8000309a:	6172676f          	jal	a4,80029eb0 <_tbss_end+0x23dec>
    8000309e:	206d                	0x206d
    800030a0:	706d6f63          	bltu	s10,t1,800037be <main+0xe66>
    800030a4:	6c69                	lui	s8,0x1a
    800030a6:	6465                	lui	s0,0x19
    800030a8:	7720                	ld	s0,104(a4)
    800030aa:	7469                	lui	s0,0xffffa
    800030ac:	2068                	fld	fa0,192(s0)
    800030ae:	67657227          	0x67657227
    800030b2:	7369                	lui	t1,0xffffa
    800030b4:	6574                	ld	a3,200(a0)
    800030b6:	2772                	fld	fa4,280(sp)
    800030b8:	6120                	ld	s0,64(a0)
    800030ba:	7474                	ld	a3,232(s0)
    800030bc:	6972                	ld	s2,280(sp)
    800030be:	7562                	ld	a0,56(sp)
    800030c0:	6574                	ld	a3,200(a0)
    800030c2:	000a                	c.slli	zero,0x2
    800030c4:	0000                	unimp
    800030c6:	0000                	unimp
    800030c8:	7250                	ld	a2,160(a2)
    800030ca:	6172676f          	jal	a4,80029ee0 <_tbss_end+0x23e1c>
    800030ce:	206d                	0x206d
    800030d0:	706d6f63          	bltu	s10,t1,800037ee <main+0xe96>
    800030d4:	6c69                	lui	s8,0x1a
    800030d6:	6465                	lui	s0,0x19
    800030d8:	7720                	ld	s0,104(a4)
    800030da:	7469                	lui	s0,0xffffa
    800030dc:	6f68                	ld	a0,216(a4)
    800030de:	7475                	lui	s0,0xffffd
    800030e0:	2720                	fld	fs0,72(a4)
    800030e2:	6572                	ld	a0,280(sp)
    800030e4:	74736967          	0x74736967
    800030e8:	7265                	lui	tp,0xffff9
    800030ea:	74612027          	fsw	ft6,1856(sp)
    800030ee:	7274                	ld	a3,224(a2)
    800030f0:	6269                	lui	tp,0x1a
    800030f2:	7475                	lui	s0,0xffffd
    800030f4:	0a65                	addi	s4,s4,25
    800030f6:	0000                	unimp
    800030f8:	6472                	ld	s0,280(sp)
    800030fa:	6c637963          	bgeu	t1,t1,800037cc <main+0xe74>
    800030fe:	2865                	addiw	a6,a6,25
    80003100:	0029                	c.nop	10
    80003102:	0000                	unimp
    80003104:	0000                	unimp
    80003106:	0000                	unimp
    80003108:	7355                	lui	t1,0xffff5
    8000310a:	6e69                	lui	t3,0x1a
    8000310c:	73252067          	0x73252067
    80003110:	202c                	fld	fa1,64(s0)
    80003112:	5a48                	lw	a0,52(a2)
    80003114:	253d                	addiw	a0,a0,15
    80003116:	0a64                	addi	s1,sp,284
	...
    80003120:	7254                	ld	a3,160(a2)
    80003122:	6979                	lui	s2,0x1e
    80003124:	676e                	ld	a4,216(sp)
    80003126:	2520                	fld	fs0,72(a0)
    80003128:	2064                	fld	fs1,192(s0)
    8000312a:	7572                	ld	a0,312(sp)
    8000312c:	736e                	ld	t1,248(sp)
    8000312e:	7420                	ld	s0,104(s0)
    80003130:	7268                	ld	a0,224(a2)
    80003132:	6867756f          	jal	a0,8007a7b8 <_tbss_end+0x746f4>
    80003136:	4420                	lw	s0,72(s0)
    80003138:	7268                	ld	a0,224(a2)
    8000313a:	7379                	lui	t1,0xffffe
    8000313c:	6f74                	ld	a3,216(a4)
    8000313e:	656e                	ld	a0,216(sp)
    80003140:	0a3a                	slli	s4,s4,0xe
    80003142:	0000                	unimp
    80003144:	0000                	unimp
    80003146:	0000                	unimp
    80003148:	654d                	lui	a0,0x13
    8000314a:	7361                	lui	t1,0xffff8
    8000314c:	7275                	lui	tp,0xffffd
    8000314e:	6465                	lui	s0,0x19
    80003150:	7420                	ld	s0,104(s0)
    80003152:	6d69                	lui	s10,0x1a
    80003154:	2065                	0x2065
    80003156:	6f74                	ld	a3,216(a4)
    80003158:	6d73206f          	j	8003602e <_tbss_end+0x2ff6a>
    8000315c:	6c61                	lui	s8,0x18
    8000315e:	206c                	fld	fa1,192(s0)
    80003160:	6f74                	ld	a3,216(a4)
    80003162:	6f20                	ld	s0,88(a4)
    80003164:	7462                	ld	s0,56(sp)
    80003166:	6961                	lui	s2,0x18
    80003168:	206e                	fld	ft0,216(sp)
    8000316a:	656d                	lui	a0,0x1b
    8000316c:	6e61                	lui	t3,0x18
    8000316e:	6e69                	lui	t3,0x1a
    80003170:	6c756667          	0x6c756667
    80003174:	7220                	ld	s0,96(a2)
    80003176:	7365                	lui	t1,0xffff9
    80003178:	6c75                	lui	s8,0x1d
    8000317a:	7374                	ld	a3,224(a4)
    8000317c:	000a                	c.slli	zero,0x2
    8000317e:	0000                	unimp
    80003180:	6946                	ld	s2,80(sp)
    80003182:	616e                	ld	sp,216(sp)
    80003184:	206c                	fld	fa1,192(s0)
    80003186:	6176                	ld	sp,344(sp)
    80003188:	756c                	ld	a1,232(a0)
    8000318a:	7365                	lui	t1,0xffff9
    8000318c:	6f20                	ld	s0,88(a4)
    8000318e:	2066                	fld	ft0,88(sp)
    80003190:	6874                	ld	a3,208(s0)
    80003192:	2065                	0x2065
    80003194:	6176                	ld	sp,344(sp)
    80003196:	6972                	ld	s2,280(sp)
    80003198:	6261                	lui	tp,0x18
    8000319a:	656c                	ld	a1,200(a0)
    8000319c:	73752073          	csrs	0x737,a0
    800031a0:	6465                	lui	s0,0x19
    800031a2:	6920                	ld	s0,80(a0)
    800031a4:	206e                	fld	ft0,216(sp)
    800031a6:	6874                	ld	a3,208(s0)
    800031a8:	2065                	0x2065
    800031aa:	6562                	ld	a0,24(sp)
    800031ac:	636e                	ld	t1,216(sp)
    800031ae:	6d68                	ld	a0,216(a0)
    800031b0:	7261                	lui	tp,0xffff8
    800031b2:	000a3a6b          	0xa3a6b
    800031b6:	0000                	unimp
    800031b8:	6e49                	lui	t3,0x12
    800031ba:	5f74                	lw	a3,124(a4)
    800031bc:	626f6c47          	fmsub.d	fs8,ft10,ft6,fa2,unknown
    800031c0:	203a                	fld	ft0,392(sp)
    800031c2:	2020                	fld	fs0,64(s0)
    800031c4:	2020                	fld	fs0,64(s0)
    800031c6:	2020                	fld	fs0,64(s0)
    800031c8:	2020                	fld	fs0,64(s0)
    800031ca:	2020                	fld	fs0,64(s0)
    800031cc:	2520                	fld	fs0,72(a0)
    800031ce:	0a64                	addi	s1,sp,284
	...
    800031d8:	2020                	fld	fs0,64(s0)
    800031da:	2020                	fld	fs0,64(s0)
    800031dc:	2020                	fld	fs0,64(s0)
    800031de:	2020                	fld	fs0,64(s0)
    800031e0:	756f6873          	csrrsi	a6,0x756,30
    800031e4:	646c                	ld	a1,200(s0)
    800031e6:	6220                	ld	s0,64(a2)
    800031e8:	3a65                	addiw	s4,s4,-7
    800031ea:	2020                	fld	fs0,64(s0)
    800031ec:	2520                	fld	fs0,72(a0)
    800031ee:	0a64                	addi	s1,sp,284
	...
    800031f8:	6f42                	ld	t5,16(sp)
    800031fa:	475f6c6f          	jal	s8,800f9e6e <_tbss_end+0xf3daa>
    800031fe:	6f6c                	ld	a1,216(a4)
    80003200:	3a62                	fld	fs4,56(sp)
    80003202:	2020                	fld	fs0,64(s0)
    80003204:	2020                	fld	fs0,64(s0)
    80003206:	2020                	fld	fs0,64(s0)
    80003208:	2020                	fld	fs0,64(s0)
    8000320a:	2020                	fld	fs0,64(s0)
    8000320c:	2520                	fld	fs0,72(a0)
    8000320e:	0a64                	addi	s1,sp,284
	...
    80003218:	315f6843          	fmadd.s	fa6,ft10,fs5,ft6,unknown
    8000321c:	475f 6f6c 3a62      	0x3a626f6c475f
    80003222:	2020                	fld	fs0,64(s0)
    80003224:	2020                	fld	fs0,64(s0)
    80003226:	2020                	fld	fs0,64(s0)
    80003228:	2020                	fld	fs0,64(s0)
    8000322a:	2020                	fld	fs0,64(s0)
    8000322c:	2520                	fld	fs0,72(a0)
    8000322e:	00000a63          	beqz	zero,80003242 <main+0x8ea>
    80003232:	0000                	unimp
    80003234:	0000                	unimp
    80003236:	0000                	unimp
    80003238:	2020                	fld	fs0,64(s0)
    8000323a:	2020                	fld	fs0,64(s0)
    8000323c:	2020                	fld	fs0,64(s0)
    8000323e:	2020                	fld	fs0,64(s0)
    80003240:	756f6873          	csrrsi	a6,0x756,30
    80003244:	646c                	ld	a1,200(s0)
    80003246:	6220                	ld	s0,64(a2)
    80003248:	3a65                	addiw	s4,s4,-7
    8000324a:	2020                	fld	fs0,64(s0)
    8000324c:	2520                	fld	fs0,72(a0)
    8000324e:	00000a63          	beqz	zero,80003262 <main+0x90a>
    80003252:	0000                	unimp
    80003254:	0000                	unimp
    80003256:	0000                	unimp
    80003258:	325f6843          	fmadd.d	fa6,ft10,ft5,ft6,unknown
    8000325c:	475f 6f6c 3a62      	0x3a626f6c475f
    80003262:	2020                	fld	fs0,64(s0)
    80003264:	2020                	fld	fs0,64(s0)
    80003266:	2020                	fld	fs0,64(s0)
    80003268:	2020                	fld	fs0,64(s0)
    8000326a:	2020                	fld	fs0,64(s0)
    8000326c:	2520                	fld	fs0,72(a0)
    8000326e:	00000a63          	beqz	zero,80003282 <main+0x92a>
    80003272:	0000                	unimp
    80003274:	0000                	unimp
    80003276:	0000                	unimp
    80003278:	7241                	lui	tp,0xffff0
    8000327a:	5f72                	lw	t5,60(sp)
    8000327c:	5f31                	li	t5,-20
    8000327e:	626f6c47          	fmsub.d	fs8,ft10,ft6,fa2,unknown
    80003282:	3a5d385b          	0x3a5d385b
    80003286:	2020                	fld	fs0,64(s0)
    80003288:	2020                	fld	fs0,64(s0)
    8000328a:	2020                	fld	fs0,64(s0)
    8000328c:	2520                	fld	fs0,72(a0)
    8000328e:	0a64                	addi	s1,sp,284
	...
    80003298:	7241                	lui	tp,0xffff0
    8000329a:	5f72                	lw	t5,60(sp)
    8000329c:	5f32                	lw	t5,44(sp)
    8000329e:	626f6c47          	fmsub.d	fs8,ft10,ft6,fa2,unknown
    800032a2:	5b5d385b          	0x5b5d385b
    800032a6:	203a5d37          	lui	s10,0x203a5
    800032aa:	2020                	fld	fs0,64(s0)
    800032ac:	2520                	fld	fs0,72(a0)
    800032ae:	0a64                	addi	s1,sp,284
	...
    800032b8:	2020                	fld	fs0,64(s0)
    800032ba:	2020                	fld	fs0,64(s0)
    800032bc:	2020                	fld	fs0,64(s0)
    800032be:	2020                	fld	fs0,64(s0)
    800032c0:	756f6873          	csrrsi	a6,0x756,30
    800032c4:	646c                	ld	a1,200(s0)
    800032c6:	6220                	ld	s0,64(a2)
    800032c8:	3a65                	addiw	s4,s4,-7
    800032ca:	2020                	fld	fs0,64(s0)
    800032cc:	4e20                	lw	s0,88(a2)
    800032ce:	6d75                	lui	s10,0x1d
    800032d0:	6562                	ld	a0,24(sp)
    800032d2:	5f72                	lw	t5,60(sp)
    800032d4:	525f664f          	fnmadd.d	fa2,ft10,ft5,fa0,unknown
    800032d8:	6e75                	lui	t3,0x1d
    800032da:	202b2073          	csrs	hedeleg,s6
    800032de:	3031                	0x3031
    800032e0:	000a                	c.slli	zero,0x2
    800032e2:	0000                	unimp
    800032e4:	0000                	unimp
    800032e6:	0000                	unimp
    800032e8:	7450                	ld	a2,168(s0)
    800032ea:	5f72                	lw	t5,60(sp)
    800032ec:	626f6c47          	fmsub.d	fs8,ft10,ft6,fa2,unknown
    800032f0:	3e2d                	addiw	t3,t3,-21
    800032f2:	000a                	c.slli	zero,0x2
    800032f4:	0000                	unimp
    800032f6:	0000                	unimp
    800032f8:	2020                	fld	fs0,64(s0)
    800032fa:	7450                	ld	a2,168(s0)
    800032fc:	5f72                	lw	t5,60(sp)
    800032fe:	706d6f43          	fmadd.s	ft10,fs10,ft6,fa4,unknown
    80003302:	203a                	fld	ft0,392(sp)
    80003304:	2020                	fld	fs0,64(s0)
    80003306:	2020                	fld	fs0,64(s0)
    80003308:	2020                	fld	fs0,64(s0)
    8000330a:	2020                	fld	fs0,64(s0)
    8000330c:	2520                	fld	fs0,72(a0)
    8000330e:	0a64                	addi	s1,sp,284
	...
    80003318:	2020                	fld	fs0,64(s0)
    8000331a:	2020                	fld	fs0,64(s0)
    8000331c:	2020                	fld	fs0,64(s0)
    8000331e:	2020                	fld	fs0,64(s0)
    80003320:	756f6873          	csrrsi	a6,0x756,30
    80003324:	646c                	ld	a1,200(s0)
    80003326:	6220                	ld	s0,64(a2)
    80003328:	3a65                	addiw	s4,s4,-7
    8000332a:	2020                	fld	fs0,64(s0)
    8000332c:	2820                	fld	fs0,80(s0)
    8000332e:	6d69                	lui	s10,0x1a
    80003330:	6c70                	ld	a2,216(s0)
    80003332:	6d65                	lui	s10,0x19
    80003334:	6e65                	lui	t3,0x19
    80003336:	6174                	ld	a3,192(a0)
    80003338:	6974                	ld	a3,208(a0)
    8000333a:	642d6e6f          	jal	t3,800d997c <_tbss_end+0xd38b8>
    8000333e:	7065                	c.lui	zero,0xffff9
    80003340:	6e65                	lui	t3,0x19
    80003342:	6564                	ld	s1,200(a0)
    80003344:	746e                	ld	s0,248(sp)
    80003346:	0a29                	addi	s4,s4,10
	...
    80003350:	2020                	fld	fs0,64(s0)
    80003352:	6944                	ld	s1,144(a0)
    80003354:	3a726373          	csrrsi	t1,0x3a7,4
    80003358:	2020                	fld	fs0,64(s0)
    8000335a:	2020                	fld	fs0,64(s0)
    8000335c:	2020                	fld	fs0,64(s0)
    8000335e:	2020                	fld	fs0,64(s0)
    80003360:	2020                	fld	fs0,64(s0)
    80003362:	2020                	fld	fs0,64(s0)
    80003364:	2520                	fld	fs0,72(a0)
    80003366:	0a64                	addi	s1,sp,284
	...
    80003370:	2020                	fld	fs0,64(s0)
    80003372:	6e45                	lui	t3,0x11
    80003374:	6d75                	lui	s10,0x1d
    80003376:	435f 6d6f 3a70      	0x3a706d6f435f
    8000337c:	2020                	fld	fs0,64(s0)
    8000337e:	2020                	fld	fs0,64(s0)
    80003380:	2020                	fld	fs0,64(s0)
    80003382:	2020                	fld	fs0,64(s0)
    80003384:	2520                	fld	fs0,72(a0)
    80003386:	0a64                	addi	s1,sp,284
	...
    80003390:	2020                	fld	fs0,64(s0)
    80003392:	6e49                	lui	t3,0x12
    80003394:	5f74                	lw	a3,124(a4)
    80003396:	706d6f43          	fmadd.s	ft10,fs10,ft6,fa4,unknown
    8000339a:	203a                	fld	ft0,392(sp)
    8000339c:	2020                	fld	fs0,64(s0)
    8000339e:	2020                	fld	fs0,64(s0)
    800033a0:	2020                	fld	fs0,64(s0)
    800033a2:	2020                	fld	fs0,64(s0)
    800033a4:	2520                	fld	fs0,72(a0)
    800033a6:	0a64                	addi	s1,sp,284
	...
    800033b0:	2020                	fld	fs0,64(s0)
    800033b2:	5f727453          	0x5f727453
    800033b6:	706d6f43          	fmadd.s	ft10,fs10,ft6,fa4,unknown
    800033ba:	203a                	fld	ft0,392(sp)
    800033bc:	2020                	fld	fs0,64(s0)
    800033be:	2020                	fld	fs0,64(s0)
    800033c0:	2020                	fld	fs0,64(s0)
    800033c2:	2020                	fld	fs0,64(s0)
    800033c4:	2520                	fld	fs0,72(a0)
    800033c6:	00000a73          	0xa73
    800033ca:	0000                	unimp
    800033cc:	0000                	unimp
    800033ce:	0000                	unimp
    800033d0:	2020                	fld	fs0,64(s0)
    800033d2:	2020                	fld	fs0,64(s0)
    800033d4:	2020                	fld	fs0,64(s0)
    800033d6:	2020                	fld	fs0,64(s0)
    800033d8:	756f6873          	csrrsi	a6,0x756,30
    800033dc:	646c                	ld	a1,200(s0)
    800033de:	6220                	ld	s0,64(a2)
    800033e0:	3a65                	addiw	s4,s4,-7
    800033e2:	2020                	fld	fs0,64(s0)
    800033e4:	4420                	lw	s0,72(s0)
    800033e6:	5248                	lw	a0,36(a2)
    800033e8:	5359                	li	t1,-10
    800033ea:	4f54                	lw	a3,28(a4)
    800033ec:	454e                	lw	a0,208(sp)
    800033ee:	5020                	lw	s0,96(s0)
    800033f0:	4f52                	lw	t5,20(sp)
    800033f2:	4d415247          	0x4d415247
    800033f6:	202c                	fld	fa1,64(s0)
    800033f8:	454d4f53          	0x454d4f53
    800033fc:	5320                	lw	s0,96(a4)
    800033fe:	5254                	lw	a3,36(a2)
    80003400:	4e49                	li	t3,18
    80003402:	00000a47          	fmsub.s	fs4,ft0,ft0,ft0,rne
    80003406:	0000                	unimp
    80003408:	654e                	ld	a0,208(sp)
    8000340a:	7478                	ld	a4,232(s0)
    8000340c:	505f 7274 475f      	0x475f7274505f
    80003412:	6f6c                	ld	a1,216(a4)
    80003414:	2d62                	fld	fs10,24(sp)
    80003416:	0a3e                	slli	s4,s4,0xf
	...
    80003420:	2020                	fld	fs0,64(s0)
    80003422:	2020                	fld	fs0,64(s0)
    80003424:	2020                	fld	fs0,64(s0)
    80003426:	2020                	fld	fs0,64(s0)
    80003428:	756f6873          	csrrsi	a6,0x756,30
    8000342c:	646c                	ld	a1,200(s0)
    8000342e:	6220                	ld	s0,64(a2)
    80003430:	3a65                	addiw	s4,s4,-7
    80003432:	2020                	fld	fs0,64(s0)
    80003434:	2820                	fld	fs0,80(s0)
    80003436:	6d69                	lui	s10,0x1a
    80003438:	6c70                	ld	a2,216(s0)
    8000343a:	6d65                	lui	s10,0x19
    8000343c:	6e65                	lui	t3,0x19
    8000343e:	6174                	ld	a3,192(a0)
    80003440:	6974                	ld	a3,208(a0)
    80003442:	642d6e6f          	jal	t3,800d9a84 <_tbss_end+0xd39c0>
    80003446:	7065                	c.lui	zero,0xffff9
    80003448:	6e65                	lui	t3,0x19
    8000344a:	6564                	ld	s1,200(a0)
    8000344c:	746e                	ld	s0,248(sp)
    8000344e:	2c29                	addiw	s8,s8,10
    80003450:	7320                	ld	s0,96(a4)
    80003452:	6d61                	lui	s10,0x18
    80003454:	2065                	0x2065
    80003456:	7361                	lui	t1,0xffff8
    80003458:	6120                	ld	s0,64(a0)
    8000345a:	6f62                	ld	t5,24(sp)
    8000345c:	6576                	ld	a0,344(sp)
    8000345e:	000a                	c.slli	zero,0x2
    80003460:	6e49                	lui	t3,0x12
    80003462:	5f74                	lw	a3,124(a4)
    80003464:	5f31                	li	t5,-20
    80003466:	6f4c                	ld	a1,152(a4)
    80003468:	20203a63          	0x20203a63
    8000346c:	2020                	fld	fs0,64(s0)
    8000346e:	2020                	fld	fs0,64(s0)
    80003470:	2020                	fld	fs0,64(s0)
    80003472:	2020                	fld	fs0,64(s0)
    80003474:	2520                	fld	fs0,72(a0)
    80003476:	0a64                	addi	s1,sp,284
	...
    80003480:	6e49                	lui	t3,0x12
    80003482:	5f74                	lw	a3,124(a4)
    80003484:	5f32                	lw	t5,44(sp)
    80003486:	6f4c                	ld	a1,152(a4)
    80003488:	20203a63          	0x20203a63
    8000348c:	2020                	fld	fs0,64(s0)
    8000348e:	2020                	fld	fs0,64(s0)
    80003490:	2020                	fld	fs0,64(s0)
    80003492:	2020                	fld	fs0,64(s0)
    80003494:	2520                	fld	fs0,72(a0)
    80003496:	0a64                	addi	s1,sp,284
	...
    800034a0:	6e49                	lui	t3,0x12
    800034a2:	5f74                	lw	a3,124(a4)
    800034a4:	6f4c5f33          	0x6f4c5f33
    800034a8:	20203a63          	0x20203a63
    800034ac:	2020                	fld	fs0,64(s0)
    800034ae:	2020                	fld	fs0,64(s0)
    800034b0:	2020                	fld	fs0,64(s0)
    800034b2:	2020                	fld	fs0,64(s0)
    800034b4:	2520                	fld	fs0,72(a0)
    800034b6:	0a64                	addi	s1,sp,284
	...
    800034c0:	6e45                	lui	t3,0x11
    800034c2:	6d75                	lui	s10,0x1d
    800034c4:	4c5f 636f 203a      	0x203a636f4c5f
    800034ca:	2020                	fld	fs0,64(s0)
    800034cc:	2020                	fld	fs0,64(s0)
    800034ce:	2020                	fld	fs0,64(s0)
    800034d0:	2020                	fld	fs0,64(s0)
    800034d2:	2020                	fld	fs0,64(s0)
    800034d4:	2520                	fld	fs0,72(a0)
    800034d6:	0a64                	addi	s1,sp,284
	...
    800034e0:	5f727453          	0x5f727453
    800034e4:	5f31                	li	t5,-20
    800034e6:	6f4c                	ld	a1,152(a4)
    800034e8:	20203a63          	0x20203a63
    800034ec:	2020                	fld	fs0,64(s0)
    800034ee:	2020                	fld	fs0,64(s0)
    800034f0:	2020                	fld	fs0,64(s0)
    800034f2:	2020                	fld	fs0,64(s0)
    800034f4:	2520                	fld	fs0,72(a0)
    800034f6:	00000a73          	0xa73
    800034fa:	0000                	unimp
    800034fc:	0000                	unimp
    800034fe:	0000                	unimp
    80003500:	2020                	fld	fs0,64(s0)
    80003502:	2020                	fld	fs0,64(s0)
    80003504:	2020                	fld	fs0,64(s0)
    80003506:	2020                	fld	fs0,64(s0)
    80003508:	756f6873          	csrrsi	a6,0x756,30
    8000350c:	646c                	ld	a1,200(s0)
    8000350e:	6220                	ld	s0,64(a2)
    80003510:	3a65                	addiw	s4,s4,-7
    80003512:	2020                	fld	fs0,64(s0)
    80003514:	4420                	lw	s0,72(s0)
    80003516:	5248                	lw	a0,36(a2)
    80003518:	5359                	li	t1,-10
    8000351a:	4f54                	lw	a3,28(a4)
    8000351c:	454e                	lw	a0,208(sp)
    8000351e:	5020                	lw	s0,96(s0)
    80003520:	4f52                	lw	t5,20(sp)
    80003522:	4d415247          	0x4d415247
    80003526:	202c                	fld	fa1,64(s0)
    80003528:	2731                	addiw	a4,a4,12
    8000352a:	53205453          	0x53205453
    8000352e:	5254                	lw	a3,36(a2)
    80003530:	4e49                	li	t3,18
    80003532:	00000a47          	fmsub.s	fs4,ft0,ft0,ft0,rne
    80003536:	0000                	unimp
    80003538:	5f727453          	0x5f727453
    8000353c:	5f32                	lw	t5,44(sp)
    8000353e:	6f4c                	ld	a1,152(a4)
    80003540:	20203a63          	0x20203a63
    80003544:	2020                	fld	fs0,64(s0)
    80003546:	2020                	fld	fs0,64(s0)
    80003548:	2020                	fld	fs0,64(s0)
    8000354a:	2020                	fld	fs0,64(s0)
    8000354c:	2520                	fld	fs0,72(a0)
    8000354e:	00000a73          	0xa73
    80003552:	0000                	unimp
    80003554:	0000                	unimp
    80003556:	0000                	unimp
    80003558:	2020                	fld	fs0,64(s0)
    8000355a:	2020                	fld	fs0,64(s0)
    8000355c:	2020                	fld	fs0,64(s0)
    8000355e:	2020                	fld	fs0,64(s0)
    80003560:	756f6873          	csrrsi	a6,0x756,30
    80003564:	646c                	ld	a1,200(s0)
    80003566:	6220                	ld	s0,64(a2)
    80003568:	3a65                	addiw	s4,s4,-7
    8000356a:	2020                	fld	fs0,64(s0)
    8000356c:	4420                	lw	s0,72(s0)
    8000356e:	5248                	lw	a0,36(a2)
    80003570:	5359                	li	t1,-10
    80003572:	4f54                	lw	a3,28(a4)
    80003574:	454e                	lw	a0,208(sp)
    80003576:	5020                	lw	s0,96(s0)
    80003578:	4f52                	lw	t5,20(sp)
    8000357a:	4d415247          	0x4d415247
    8000357e:	202c                	fld	fa1,64(s0)
    80003580:	2732                	fld	fa4,264(sp)
    80003582:	444e                	lw	s0,208(sp)
    80003584:	5320                	lw	s0,96(a4)
    80003586:	5254                	lw	a3,36(a2)
    80003588:	4e49                	li	t3,18
    8000358a:	00000a47          	fmsub.s	fs4,ft0,ft0,ft0,rne
    8000358e:	0000                	unimp
    80003590:	694d                	lui	s2,0x13
    80003592:	736f7263          	bgeu	t5,s6,80003cb6 <Arr_2_Glob+0x39e>
    80003596:	6365                	lui	t1,0x19
    80003598:	73646e6f          	jal	t3,80049cce <_tbss_end+0x43c0a>
    8000359c:	6620                	ld	s0,72(a2)
    8000359e:	6f20726f          	jal	tp,8000ac90 <_tbss_end+0x4bcc>
    800035a2:	656e                	ld	a0,216(sp)
    800035a4:	7220                	ld	s0,96(a2)
    800035a6:	6e75                	lui	t3,0x1d
    800035a8:	7420                	ld	s0,104(s0)
    800035aa:	7268                	ld	a0,224(a2)
    800035ac:	6867756f          	jal	a0,8007ac32 <_tbss_end+0x74b6e>
    800035b0:	4420                	lw	s0,72(s0)
    800035b2:	7268                	ld	a0,224(a2)
    800035b4:	7379                	lui	t1,0xffffe
    800035b6:	6f74                	ld	a3,216(a4)
    800035b8:	656e                	ld	a0,216(sp)
    800035ba:	203a                	fld	ft0,392(sp)
    800035bc:	6c25                	lui	s8,0x9
    800035be:	0a64                	addi	s1,sp,284
	...
    800035c8:	6844                	ld	s1,144(s0)
    800035ca:	7972                	ld	s2,312(sp)
    800035cc:	6e6f7473          	csrrci	s0,0x6e6,30
    800035d0:	7365                	lui	t1,0xffff9
    800035d2:	7020                	ld	s0,96(s0)
    800035d4:	7265                	lui	tp,0xffff9
    800035d6:	5320                	lw	s0,96(a4)
    800035d8:	6365                	lui	t1,0x19
    800035da:	3a646e6f          	jal	t3,80049980 <_tbss_end+0x438bc>
    800035de:	2020                	fld	fs0,64(s0)
    800035e0:	2020                	fld	fs0,64(s0)
    800035e2:	2020                	fld	fs0,64(s0)
    800035e4:	2020                	fld	fs0,64(s0)
    800035e6:	2020                	fld	fs0,64(s0)
    800035e8:	2020                	fld	fs0,64(s0)
    800035ea:	2020                	fld	fs0,64(s0)
    800035ec:	2020                	fld	fs0,64(s0)
    800035ee:	2020                	fld	fs0,64(s0)
    800035f0:	2020                	fld	fs0,64(s0)
    800035f2:	2020                	fld	fs0,64(s0)
    800035f4:	6c25                	lui	s8,0x9
    800035f6:	0a64                	addi	s1,sp,284
	...
    80003600:	4844                	lw	s1,20(s0)
    80003602:	5952                	lw	s2,52(sp)
    80003604:	4e4f5453          	0x4e4f5453
    80003608:	2045                	0x2045
    8000360a:	5250                	lw	a2,36(a2)
    8000360c:	4152474f          	fnmadd.s	fa4,ft4,fs5,fs0,rmm
    80003610:	2c4d                	addiw	s8,s8,19
    80003612:	3120                	fld	fs0,96(a0)
    80003614:	20545327          	0x20545327
    80003618:	49525453          	0x49525453
    8000361c:	474e                	lw	a4,208(sp)
    8000361e:	0000                	unimp
    80003620:	4844                	lw	s1,20(s0)
    80003622:	5952                	lw	s2,52(sp)
    80003624:	4e4f5453          	0x4e4f5453
    80003628:	2045                	0x2045
    8000362a:	5250                	lw	a2,36(a2)
    8000362c:	4152474f          	fnmadd.s	fa4,ft4,fs5,fs0,rmm
    80003630:	2c4d                	addiw	s8,s8,19
    80003632:	3220                	fld	fs0,96(a2)
    80003634:	20444e27          	fsq	ft4,540(s0) # 1921c <buflen.1+0x191dc>
    80003638:	49525453          	0x49525453
    8000363c:	474e                	lw	a4,208(sp)
    8000363e:	0000                	unimp
    80003640:	4844                	lw	s1,20(s0)
    80003642:	5952                	lw	s2,52(sp)
    80003644:	4e4f5453          	0x4e4f5453
    80003648:	2045                	0x2045
    8000364a:	5250                	lw	a2,36(a2)
    8000364c:	4152474f          	fnmadd.s	fa4,ft4,fs5,fs0,rmm
    80003650:	2c4d                	addiw	s8,s8,19
    80003652:	3320                	fld	fs0,96(a4)
    80003654:	20445227          	0x20445227
    80003658:	49525453          	0x49525453
    8000365c:	474e                	lw	a4,208(sp)
    8000365e:	0000                	unimp
    80003660:	6e28                	ld	a0,88(a2)
    80003662:	6c75                	lui	s8,0x1d
    80003664:	296c                	fld	fa1,208(a0)
    80003666:	0000                	unimp
    80003668:	636d                	lui	t1,0x1b
    8000366a:	6379                	lui	t1,0x1e
    8000366c:	656c                	ld	a1,200(a0)
    8000366e:	0000                	unimp
    80003670:	696d                	lui	s2,0x1b
    80003672:	736e                	ld	t1,248(sp)
    80003674:	7274                	ld	a3,224(a2)
    80003676:	7465                	lui	s0,0xffff9
	...
    80003680:	6d49                	lui	s10,0x12
    80003682:	6c70                	ld	a2,216(s0)
    80003684:	6d65                	lui	s10,0x19
    80003686:	6e65                	lui	t3,0x19
    80003688:	2074                	fld	fa3,192(s0)
    8000368a:	616d                	addi	sp,sp,240
    8000368c:	6e69                	lui	t3,0x1a
    8000368e:	2928                	fld	fa0,80(a0)
    80003690:	202c                	fld	fa1,64(s0)
    80003692:	6f66                	ld	t5,88(sp)
    80003694:	000a216f          	jal	sp,800a5694 <_tbss_end+0x9f5d0>
    80003698:	7325                	lui	t1,0xfffe9
    8000369a:	3d20                	fld	fs0,120(a0)
    8000369c:	2520                	fld	fs0,72(a0)
    8000369e:	0a64                	addi	s1,sp,284
	...

Disassembly of section .rodata:

00000000800036a4 <__global_pointer$-0x958>:
    800036a4:	ecba                	sd	a4,88(sp)
    800036a6:	ffff                	0xffff
    800036a8:	ec68                	sd	a0,216(s0)
    800036aa:	ffff                	0xffff
    800036ac:	ecc2                	sd	a6,88(sp)
    800036ae:	ffff                	0xffff
    800036b0:	ec68                	sd	a0,216(s0)
    800036b2:	ffff                	0xffff
    800036b4:	ec68                	sd	a0,216(s0)
    800036b6:	ffff                	0xffff
    800036b8:	ec68                	sd	a0,216(s0)
    800036ba:	ffff                	0xffff
    800036bc:	ec68                	sd	a0,216(s0)
    800036be:	ffff                	0xffff
    800036c0:	eccc                	sd	a1,152(s1)
    800036c2:	ffff                	0xffff
    800036c4:	ec68                	sd	a0,216(s0)
    800036c6:	ffff                	0xffff
    800036c8:	ec68                	sd	a0,216(s0)
    800036ca:	ffff                	0xffff
    800036cc:	ec50                	sd	a2,152(s0)
    800036ce:	ffff                	0xffff
    800036d0:	ee28                	sd	a0,88(a2)
    800036d2:	ffff                	0xffff
    800036d4:	ec68                	sd	a0,216(s0)
    800036d6:	ffff                	0xffff
    800036d8:	ec50                	sd	a2,152(s0)
    800036da:	ffff                	0xffff
    800036dc:	ec74                	sd	a3,216(s0)
    800036de:	ffff                	0xffff
    800036e0:	ec74                	sd	a3,216(s0)
    800036e2:	ffff                	0xffff
    800036e4:	ec74                	sd	a3,216(s0)
    800036e6:	ffff                	0xffff
    800036e8:	ec74                	sd	a3,216(s0)
    800036ea:	ffff                	0xffff
    800036ec:	ec74                	sd	a3,216(s0)
    800036ee:	ffff                	0xffff
    800036f0:	ec74                	sd	a3,216(s0)
    800036f2:	ffff                	0xffff
    800036f4:	ec74                	sd	a3,216(s0)
    800036f6:	ffff                	0xffff
    800036f8:	ec74                	sd	a3,216(s0)
    800036fa:	ffff                	0xffff
    800036fc:	ec74                	sd	a3,216(s0)
    800036fe:	ffff                	0xffff
    80003700:	ec68                	sd	a0,216(s0)
    80003702:	ffff                	0xffff
    80003704:	ec68                	sd	a0,216(s0)
    80003706:	ffff                	0xffff
    80003708:	ec68                	sd	a0,216(s0)
    8000370a:	ffff                	0xffff
    8000370c:	ec68                	sd	a0,216(s0)
    8000370e:	ffff                	0xffff
    80003710:	ec68                	sd	a0,216(s0)
    80003712:	ffff                	0xffff
    80003714:	ec68                	sd	a0,216(s0)
    80003716:	ffff                	0xffff
    80003718:	ec68                	sd	a0,216(s0)
    8000371a:	ffff                	0xffff
    8000371c:	ec68                	sd	a0,216(s0)
    8000371e:	ffff                	0xffff
    80003720:	ec68                	sd	a0,216(s0)
    80003722:	ffff                	0xffff
    80003724:	ec68                	sd	a0,216(s0)
    80003726:	ffff                	0xffff
    80003728:	ec68                	sd	a0,216(s0)
    8000372a:	ffff                	0xffff
    8000372c:	ec68                	sd	a0,216(s0)
    8000372e:	ffff                	0xffff
    80003730:	ec68                	sd	a0,216(s0)
    80003732:	ffff                	0xffff
    80003734:	ec68                	sd	a0,216(s0)
    80003736:	ffff                	0xffff
    80003738:	ec68                	sd	a0,216(s0)
    8000373a:	ffff                	0xffff
    8000373c:	ec68                	sd	a0,216(s0)
    8000373e:	ffff                	0xffff
    80003740:	ec68                	sd	a0,216(s0)
    80003742:	ffff                	0xffff
    80003744:	ec68                	sd	a0,216(s0)
    80003746:	ffff                	0xffff
    80003748:	ec68                	sd	a0,216(s0)
    8000374a:	ffff                	0xffff
    8000374c:	ec68                	sd	a0,216(s0)
    8000374e:	ffff                	0xffff
    80003750:	ec68                	sd	a0,216(s0)
    80003752:	ffff                	0xffff
    80003754:	ec68                	sd	a0,216(s0)
    80003756:	ffff                	0xffff
    80003758:	ec68                	sd	a0,216(s0)
    8000375a:	ffff                	0xffff
    8000375c:	ec68                	sd	a0,216(s0)
    8000375e:	ffff                	0xffff
    80003760:	ec68                	sd	a0,216(s0)
    80003762:	ffff                	0xffff
    80003764:	ec68                	sd	a0,216(s0)
    80003766:	ffff                	0xffff
    80003768:	ec68                	sd	a0,216(s0)
    8000376a:	ffff                	0xffff
    8000376c:	ec68                	sd	a0,216(s0)
    8000376e:	ffff                	0xffff
    80003770:	ec68                	sd	a0,216(s0)
    80003772:	ffff                	0xffff
    80003774:	ec68                	sd	a0,216(s0)
    80003776:	ffff                	0xffff
    80003778:	ec68                	sd	a0,216(s0)
    8000377a:	ffff                	0xffff
    8000377c:	ec68                	sd	a0,216(s0)
    8000377e:	ffff                	0xffff
    80003780:	ec68                	sd	a0,216(s0)
    80003782:	ffff                	0xffff
    80003784:	ec68                	sd	a0,216(s0)
    80003786:	ffff                	0xffff
    80003788:	ec68                	sd	a0,216(s0)
    8000378a:	ffff                	0xffff
    8000378c:	ec68                	sd	a0,216(s0)
    8000378e:	ffff                	0xffff
    80003790:	ec68                	sd	a0,216(s0)
    80003792:	ffff                	0xffff
    80003794:	ec68                	sd	a0,216(s0)
    80003796:	ffff                	0xffff
    80003798:	ec68                	sd	a0,216(s0)
    8000379a:	ffff                	0xffff
    8000379c:	ec68                	sd	a0,216(s0)
    8000379e:	ffff                	0xffff
    800037a0:	ec68                	sd	a0,216(s0)
    800037a2:	ffff                	0xffff
    800037a4:	ee3e                	sd	a5,280(sp)
    800037a6:	ffff                	0xffff
    800037a8:	ed58                	sd	a4,152(a0)
    800037aa:	ffff                	0xffff
    800037ac:	ec68                	sd	a0,216(s0)
    800037ae:	ffff                	0xffff
    800037b0:	ec68                	sd	a0,216(s0)
    800037b2:	ffff                	0xffff
    800037b4:	ec68                	sd	a0,216(s0)
    800037b6:	ffff                	0xffff
    800037b8:	ec68                	sd	a0,216(s0)
    800037ba:	ffff                	0xffff
    800037bc:	ec68                	sd	a0,216(s0)
    800037be:	ffff                	0xffff
    800037c0:	ec68                	sd	a0,216(s0)
    800037c2:	ffff                	0xffff
    800037c4:	ec68                	sd	a0,216(s0)
    800037c6:	ffff                	0xffff
    800037c8:	ed74                	sd	a3,216(a0)
    800037ca:	ffff                	0xffff
    800037cc:	ec68                	sd	a0,216(s0)
    800037ce:	ffff                	0xffff
    800037d0:	ec68                	sd	a0,216(s0)
    800037d2:	ffff                	0xffff
    800037d4:	ee58                	sd	a4,152(a2)
    800037d6:	ffff                	0xffff
    800037d8:	ecda                	sd	s6,88(sp)
    800037da:	ffff                	0xffff
    800037dc:	ec68                	sd	a0,216(s0)
    800037de:	ffff                	0xffff
    800037e0:	ec68                	sd	a0,216(s0)
    800037e2:	ffff                	0xffff
    800037e4:	ed94                	sd	a3,24(a1)
    800037e6:	ffff                	0xffff
    800037e8:	ec68                	sd	a0,216(s0)
    800037ea:	ffff                	0xffff
    800037ec:	ee5c                	sd	a5,152(a2)
    800037ee:	ffff                	0xffff
    800037f0:	ec68                	sd	a0,216(s0)
    800037f2:	ffff                	0xffff
    800037f4:	ec68                	sd	a0,216(s0)
    800037f6:	ffff                	0xffff
    800037f8:	ed7e                	sd	t6,152(sp)
    800037fa:	ffff                	0xffff

Disassembly of section .sbss:

0000000080003800 <Dhrystones_Per_Second>:
	...

0000000080003808 <Microseconds>:
	...

0000000080003810 <User_Time>:
	...

0000000080003818 <End_Time>:
	...

0000000080003820 <Begin_Time>:
	...

0000000080003828 <Done>:
    80003828:	0000                	unimp
	...

000000008000382c <Reg>:
    8000382c:	0000                	unimp
	...

0000000080003830 <Ch_2_Glob>:
	...

0000000080003831 <Ch_1_Glob>:
    80003831:	0000                	unimp
	...

0000000080003834 <Bool_Glob>:
    80003834:	0000                	unimp
	...

0000000080003838 <Int_Glob>:
	...

0000000080003840 <Next_Ptr_Glob>:
	...

0000000080003848 <Ptr_Glob>:
	...

Disassembly of section .bss:

0000000080003850 <Arr_1_Glob>:
	...

0000000080003918 <Arr_2_Glob>:
	...

0000000080006028 <counters>:
	...

0000000080006038 <counter_names>:
	...

Disassembly of section .tbss:

0000000080006080 <buflen.1+0x80006040>:
	...

Disassembly of section .comment:

0000000000000000 <.comment>:
   0:	3a434347          	fmsub.d	ft6,ft6,ft4,ft7,rmm
   4:	2820                	fld	fs0,80(s0)
   6:	29554e47          	fmsub.s	ft8,fa0,fs5,ft5,rmm
   a:	3120                	fld	fs0,96(a0)
   c:	2e31                	addiw	t3,t3,12
   e:	2e31                	addiw	t3,t3,12
  10:	0030                	addi	a2,sp,8

Disassembly of section .riscv.attributes:

0000000000000000 <.riscv.attributes>:
   0:	2941                	addiw	s2,s2,16
   2:	0000                	unimp
   4:	7200                	ld	s0,32(a2)
   6:	7369                	lui	t1,0xffffa
   8:	01007663          	bgeu	zero,a6,14 <buf.2+0x14>
   c:	001f 0000 1004      	0x10040000001f
  12:	7205                	lui	tp,0xfffe1
  14:	3676                	fld	fa2,376(sp)
  16:	6934                	ld	a3,80(a0)
  18:	7032                	0x7032
  1a:	5f30                	lw	a2,120(a4)
  1c:	326d                	addiw	tp,tp,-5
  1e:	3070                	fld	fa2,224(s0)
  20:	635f 7032 0030      	0x307032635f
  26:	0108                	addi	a0,sp,128
  28:	0b0a                	slli	s6,s6,0x2
