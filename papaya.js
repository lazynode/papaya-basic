const ethers = require(`ethers`);
const crypto = require(`crypto`);

const syncnodes = node => Object.assign(node, { $ancestor$: undefined, $children$: (node.$children$ ?? []).map(v => syncnodes(v)).map(v => Object.assign(v, { $ancestor$: node })) });
const recursion = c => node => (n => ({ ...n, $children$: (n.$children$ ?? []).map(v => recursion(c)(v)) }))(node.$nodename$ === c.name ? c(node, node) : node);
const getleaves = node => node.$children$.length ? node.$children$.map(getleaves).flat() : [node];
const getroooot = node => node.$ancestor$ ? getroooot(node.$ancestor$) : node;
const setoffset = (children) => children.reverse().forEach((v, i) => v.stackoffset = i);
const padnumlen = (a, l) => ethers.utils.hexDataLength(a) <= l ? ethers.utils.hexZeroPad(ethers.utils.hexlify(a), l) : console.log(`[ERROR]: increase tagsize to allow larger program counter`) ?? ethers.utils.hexZeroPad('0x', l);
const cleannode = nodes => { const start = nodes.findIndex(v => v.$nodename$ === 'AssemblySTOP' || v.$nodename$ === 'AssemblyRETURN' || v.$nodename$ === 'AssemblyREVERT' || v.$nodename$ === 'AssemblySELFDESTRUCT') + 1; if (start > 0) { const end = nodes.slice(start).findIndex(v => v.$nodename$ === 'AssemblyJUMPDEST' || v.$nodename$ === 'AssemblyPlaceTag'); return end < 0 ? [...nodes.slice(0, start)] : [...nodes.slice(0, start), ...cleannode(nodes.slice(start).slice(end))]; } return nodes; }
const getvaridx = (name, node, v = 0) => node === undefined ? (console.log(`varname ${name} not found`) ?? 0) : node.$nodename$ !== `BasicStatementScope` ? getvaridx(name, node.$ancestor$, v) : node.varname === name ? v : getvaridx(name, node.$ancestor$, v + 1);
const fixvaridx = (node, v) => node.$ancestor$.$nodename$ !== `BasicStatementScope` ? fixvaridx(node.$ancestor$, v) : node.$ancestor$.$children$[0] === node ? v - 1 : v;
const calstackoffset = node => { if (node.stackoffset !== undefined) { return node.stackoffset; } if (node.$ancestor$ === undefined) { return 0; } const poffset = calstackoffset(node.$ancestor$); return node.$ancestor$.$children$.length - 1 - node.$ancestor$.$children$.indexOf(node) + poffset; }

const BasicExpressionVarSwap = ({ $ancestor$, $children$, x, y }, ref) => ((vx, vy) => vx > 15 || vy > 15 || vx == vy ? { $nodename$: `AssemblySection` } : { $nodename$: `BasicStatementStackSwap`, x: vx, y: vy })(getvaridx(x, $ancestor$), getvaridx(y, $ancestor$));
const BasicExpressionVarSet = ({ $ancestor$, $children$, varname }) => ({ $nodename$: `BasicStatementStackSet`, $children$: [...$children$], index: getvaridx(varname, $ancestor$) });
const BasicExpressionVar = ({ $ancestor$, $children$, varname }, ref) => ({ $nodename$: `BasicExpressionStack`, index: fixvaridx(ref, getvaridx(varname, $ancestor$)) });
const BasicStatementWhile = ({ $ancestor$, $children$, tagsize }) => (uuid => ({ $nodename$: `BasicClean`, $children$: [{ $nodename$: `AssemblyJumpTo`, tag: uuid, tagsize: tagsize }, { $nodename$: `BasicStatementDowhile`, $children$: [...$children$, { $nodename$: `AssemblyPlaceTag`, tag: uuid }], tagsize: tagsize }] }))(crypto.randomUUID());
const BasicStatementIf = ({ $ancestor$, $children$, tagsize }) => ({ $nodename$: `BasicStatementUnless`, $children$: [{ $nodename$: `BasicExpressionISZERO`, $children$: [$children$[0]], stackoffset: 0 }, ...$children$.slice(1)], tagsize: tagsize });
const BasicStatementUnless = ({ $ancestor$, $children$, tagsize }) => (uuid => ({ $nodename$: `BasicClean`, $children$: [{ ...$children$[0], stackoffset: 0 }, { $nodename$: `AssemblyJumpIf`, tag: uuid, tagsize: tagsize }, ...$children$.slice(1), { $nodename$: `AssemblyPlaceTag`, tag: uuid }] }))(crypto.randomUUID());
const BasicStatementDowhile = ({ $ancestor$, $children$, tagsize }) => (uuid => ({ $nodename$: `BasicClean`, $children$: [{ $nodename$: `AssemblyPlaceTag`, tag: uuid }, ...$children$.slice(1), { ...$children$[0], stackoffset: 0 }, { $nodename$: `AssemblyJumpIf`, tag: uuid, tagsize: tagsize }] }))(crypto.randomUUID());
const BasicStatementScope = ({ $ancestor$, $children$, varname }) => ({ $nodename$: `BasicClean`, $children$: [{ ...$children$[0], stackoffset: 0 }, ...$children$.slice(1), { $nodename$: `AssemblyPOP` }] });
const BasicStatementSection = ({ $ancestor$, $children$ }) => ({ $nodename$: `BasicClean`, $children$: [...$children$] });
const BasicStatementStacktopRightADD = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySWAP1` }, { $nodename$: `AssemblyADD` }] };
const BasicStatementStacktopRightMUL = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySWAP1` }, { $nodename$: `AssemblyMUL` }] };
const BasicStatementStacktopRightSUB = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySWAP1` }, { $nodename$: `AssemblySUB` }] };
const BasicStatementStacktopRightDIV = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySWAP1` }, { $nodename$: `AssemblyDIV` }] };
const BasicStatementStacktopRightSDIV = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySWAP1` }, { $nodename$: `AssemblySDIV` }] };
const BasicStatementStacktopRightMOD = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySWAP1` }, { $nodename$: `AssemblyMOD` }] };
const BasicStatementStacktopRightSMOD = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySWAP1` }, { $nodename$: `AssemblySMOD` }] };
const BasicStatementStacktopRightEXP = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySWAP1` }, { $nodename$: `AssemblyEXP` }] };
const BasicStatementStacktopRightSIGNEXTEND = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySWAP1` }, { $nodename$: `AssemblySIGNEXTEND` }] };
const BasicStatementStacktopRightLT = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySWAP1` }, { $nodename$: `AssemblyLT` }] };
const BasicStatementStacktopRightGT = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySWAP1` }, { $nodename$: `AssemblyGT` }] };
const BasicStatementStacktopRightSLT = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySWAP1` }, { $nodename$: `AssemblySLT` }] };
const BasicStatementStacktopRightSGT = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySWAP1` }, { $nodename$: `AssemblySGT` }] };
const BasicStatementStacktopRightEQ = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySWAP1` }, { $nodename$: `AssemblyEQ` }] };
const BasicStatementStacktopRightAND = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySWAP1` }, { $nodename$: `AssemblyAND` }] };
const BasicStatementStacktopRightOR = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySWAP1` }, { $nodename$: `AssemblyOR` }] };
const BasicStatementStacktopRightXOR = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySWAP1` }, { $nodename$: `AssemblyXOR` }] };
const BasicStatementStacktopRightBYTE = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySWAP1` }, { $nodename$: `AssemblyBYTE` }] };
const BasicStatementStacktopRightSHL = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySWAP1` }, { $nodename$: `AssemblySHL` }] };
const BasicStatementStacktopRightSHR = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySWAP1` }, { $nodename$: `AssemblySHR` }] };
const BasicStatementStacktopRightSAR = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySWAP1` }, { $nodename$: `AssemblySAR` }] };
const BasicStatementStacktopRightSHA3 = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySWAP1` }, { $nodename$: `AssemblySHA3` }] };
const BasicStatementStacktopLeftADD = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblyADD` }] };
const BasicStatementStacktopLeftMUL = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblyMUL` }] };
const BasicStatementStacktopLeftSUB = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySUB` }] };
const BasicStatementStacktopLeftDIV = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblyDIV` }] };
const BasicStatementStacktopLeftSDIV = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySDIV` }] };
const BasicStatementStacktopLeftMOD = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblyMOD` }] };
const BasicStatementStacktopLeftSMOD = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySMOD` }] };
const BasicStatementStacktopLeftEXP = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblyEXP` }] };
const BasicStatementStacktopLeftSIGNEXTEND = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySIGNEXTEND` }] };
const BasicStatementStacktopLeftLT = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblyLT` }] };
const BasicStatementStacktopLeftGT = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblyGT` }] };
const BasicStatementStacktopLeftSLT = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySLT` }] };
const BasicStatementStacktopLeftSGT = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySGT` }] };
const BasicStatementStacktopLeftEQ = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblyEQ` }] };
const BasicStatementStacktopLeftAND = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblyAND` }] };
const BasicStatementStacktopLeftOR = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblyOR` }] };
const BasicStatementStacktopLeftXOR = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblyXOR` }] };
const BasicStatementStacktopLeftBYTE = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblyBYTE` }] };
const BasicStatementStacktopLeftSHL = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySHL` }] };
const BasicStatementStacktopLeftSHR = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySHR` }] };
const BasicStatementStacktopLeftSAR = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySAR` }] };
const BasicStatementStacktopLeftSHA3 = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySHA3` }] };
const BasicStatementStacktopISZERO = ({ $ancestor$, $children$ }) => ({ $nodename$: `AssemblySection`, $children$: [{ $nodename$: `AssemblyISZERO` }] });
const BasicStatementStacktopNOT = ({ $ancestor$, $children$ }) => ({ $nodename$: `AssemblySection`, $children$: [{ $nodename$: `AssemblyNOT` }] });
const BasicStatementStacktopBALANCE = ({ $ancestor$, $children$ }) => ({ $nodename$: `AssemblySection`, $children$: [{ $nodename$: `AssemblyBALANCE` }] });
const BasicStatementStacktopCALLDATALOAD = ({ $ancestor$, $children$ }) => ({ $nodename$: `AssemblySection`, $children$: [{ $nodename$: `AssemblyCALLDATALOAD` }] });
const BasicStatementStacktopEXTCODESIZE = ({ $ancestor$, $children$ }) => ({ $nodename$: `AssemblySection`, $children$: [{ $nodename$: `AssemblyEXTCODESIZE` }] });
const BasicStatementStacktopEXTCODEHASH = ({ $ancestor$, $children$ }) => ({ $nodename$: `AssemblySection`, $children$: [{ $nodename$: `AssemblyEXTCODEHASH` }] });
const BasicStatementStacktopBLOCKHASH = ({ $ancestor$, $children$ }) => ({ $nodename$: `AssemblySection`, $children$: [{ $nodename$: `AssemblyBLOCKHASH` }] });
const BasicStatementStacktopMLOAD = ({ $ancestor$, $children$ }) => ({ $nodename$: `AssemblySection`, $children$: [{ $nodename$: `AssemblyMLOAD` }] });
const BasicStatementStacktopSLOAD = ({ $ancestor$, $children$ }) => ({ $nodename$: `AssemblySection`, $children$: [{ $nodename$: `AssemblySLOAD` }] });
const BasicStatementStackSwap = ({ $ancestor$, $children$, x, y }) => (x === 0 || y === 0) ? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySWAP${x + y}` }] } : { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySWAP${x}` }, { $nodename$: `AssemblySWAP${y}` }, { $nodename$: `AssemblySWAP${x}` }] };
const BasicStatementStackSet = ({ $ancestor$, $children$, index }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySWAP${index + 1}` }, { $nodename$: `AssemblyPOP` }] };
const BasicStatementMemorySetByte = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblyMSTORE8` }] };
const BasicStatementMemorySet = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblyMSTORE` }] };
const BasicStatementStorageSet = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySSTORE` }] };
const BasicStatementCalldataCopy = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblyCALLDATACOPY` }] };
const BasicStatementReturndataCopy = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblyRETURNDATACOPY` }] };
const BasicStatementCodeCopy = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblyCODECOPY` }] };
const BasicStatementExtcodeCopy = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblyEXTCODECOPY` }] };
const BasicStatementDrop = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblyPOP` }] };
const BasicStatementLog = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblyLOG${$children$.length - 2}` }] };
const BasicStatementSelfDestruct = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySELFDESTRUCT` }] };
const BasicStatementRevert = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblyREVERT` }] };
const BasicStatementReturn = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblyRETURN` }] };
const BasicStatementStop = ({ $ancestor$, $children$ }) => setoffset($children$) ?? { $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySTOP` }] };
const BasicExpressionStack = ({ $ancestor$, $children$, index, stackoffset }, ref) => (index + calstackoffset(ref) + 1 > 16 ? console.log(`[ERROR]: stack overflow, reduce stack variable depth`) : null) ?? ({ $nodename$: `AssemblyDUP${index + calstackoffset(ref) + 1}` });
const BasicExpressionPlaceholder = ({ $ancestor$, $children$ }) => ({ $nodename$: `AssemblyCHAINID` });
const BasicExpressionLiteral = ({ $ancestor$, $children$, value }) => ({ $nodename$: `AssemblyPUSH`, pushdata: value });
const BasicExpressionADD = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblyADD` }] });
const BasicExpressionMUL = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblyMUL` }] });
const BasicExpressionSUB = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblySUB` }] });
const BasicExpressionDIV = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblyDIV` }] });
const BasicExpressionSDIV = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblySDIV` }] });
const BasicExpressionMOD = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblyMOD` }] });
const BasicExpressionSMOD = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblySMOD` }] });
const BasicExpressionEXP = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblyEXP` }] });
const BasicExpressionSIGNEXTEND = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblySIGNEXTEND` }] });
const BasicExpressionLT = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblyLT` }] });
const BasicExpressionGT = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblyGT` }] });
const BasicExpressionSLT = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblySLT` }] });
const BasicExpressionSGT = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblySGT` }] });
const BasicExpressionEQ = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblyEQ` }] });
const BasicExpressionAND = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblyAND` }] });
const BasicExpressionOR = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblyOR` }] });
const BasicExpressionXOR = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblyXOR` }] });
const BasicExpressionBYTE = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblyBYTE` }] });
const BasicExpressionSHL = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblySHL` }] });
const BasicExpressionSHR = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblySHR` }] });
const BasicExpressionSAR = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblySAR` }] });
const BasicExpressionSHA3 = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblySHA3` }] });
const BasicExpressionADDMOD = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblyADDMOD` }] });
const BasicExpressionMULMOD = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblyMULMOD` }] });
const BasicExpressionCREATE = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblyCREATE` }] });
const BasicExpressionCREATE2 = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblyCREATE2` }] });
const BasicExpressionDELEGATECALL = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblyDELEGATECALL` }] });
const BasicExpressionSTATICCALL = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblySTATICCALL` }] });
const BasicExpressionCALL = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblyCALL` }] });
const BasicExpressionCALLCODE = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblyCALLCODE` }] });
const BasicExpressionISZERO = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblyISZERO` }] });
const BasicExpressionNOT = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblyNOT` }] });
const BasicExpressionBALANCE = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblyBALANCE` }] });
const BasicExpressionCALLDATALOAD = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblyCALLDATALOAD` }] });
const BasicExpressionEXTCODESIZE = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblyEXTCODESIZE` }] });
const BasicExpressionEXTCODEHASH = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblyEXTCODEHASH` }] });
const BasicExpressionBLOCKHASH = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblyBLOCKHASH` }] });
const BasicExpressionMLOAD = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblyMLOAD` }] });
const BasicExpressionSLOAD = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...[...$children$].reverse(), { $nodename$: `AssemblySLOAD` }] });
const BasicExpressionADDRESS = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [{ $nodename$: `AssemblyADDRESS` }] });
const BasicExpressionORIGIN = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [{ $nodename$: `AssemblyORIGIN` }] });
const BasicExpressionCALLER = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [{ $nodename$: `AssemblyCALLER` }] });
const BasicExpressionCALLVALUE = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [{ $nodename$: `AssemblyCALLVALUE` }] });
const BasicExpressionCALLDATASIZE = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [{ $nodename$: `AssemblyCALLDATASIZE` }] });
const BasicExpressionCODESIZE = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [{ $nodename$: `AssemblyCODESIZE` }] });
const BasicExpressionGASPRICE = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [{ $nodename$: `AssemblyGASPRICE` }] });
const BasicExpressionRETURNDATASIZE = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [{ $nodename$: `AssemblyRETURNDATASIZE` }] });
const BasicExpressionCODEBASE = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [{ $nodename$: `AssemblyCODEBASE` }] });
const BasicExpressionTIMESTAMP = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [{ $nodename$: `AssemblyTIMESTAMP` }] });
const BasicExpressionNUMBER = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [{ $nodename$: `AssemblyNUMBER` }] });
const BasicExpressionDIFFICULTY = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [{ $nodename$: `AssemblyDIFFICULTY` }] });
const BasicExpressionGASLIMIT = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [{ $nodename$: `AssemblyGASLIMIT` }] });
const BasicExpressionCHAINID = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [{ $nodename$: `AssemblyCHAINID` }] });
const BasicExpressionSELFBALANCE = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [{ $nodename$: `AssemblySELFBALANCE` }] });
const BasicExpressionBASEFEE = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [{ $nodename$: `AssemblyBASEFEE` }] });
const BasicExpressionPC = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [{ $nodename$: `AssemblyPC` }] });
const BasicExpressionMSIZE = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [{ $nodename$: `AssemblyMSIZE` }] });
const BasicExpressionGAS = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [{ $nodename$: `AssemblyGAS` }] });
const BasicExpressionStorage = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblySLOAD` }] });
const BasicExpressionMemory = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblyMLOAD` }] });
const BasicExpressionCalldata = ({ $ancestor$, $children$, stackoffset }) => ({ $nodename$: `AssemblySection`, $children$: [...$children$, { $nodename$: `AssemblyCALLDATALOAD` }] });
const BasicClean = ({ $ancestor$, $children$ }, ref) => ({ $nodename$: `AssemblySection`, $children$: cleannode([...$children$.map(getleaves).flat()]) });
const AssemblyJumpIf = ({ $ancestor$, $children$, tag, tagsize }) => ({ $nodename$: `WriteData`, $children$: [{ $nodename$: `AssemblyPushTag`, tag: tag, tagsize: tagsize }, { $nodename$: `AssemblyJUMPI` }] });
const AssemblyJumpTo = ({ $ancestor$, $children$, tag, tagsize }) => ({ $nodename$: `WriteData`, $children$: [{ $nodename$: `AssemblyPushTag`, tag: tag, tagsize: tagsize }, { $nodename$: `AssemblyJUMP` }] });
const AssemblyPushTag = ({ $ancestor$, $children$, tag, tagsize }) => ({ $nodename$: `WriteData`, $children$: [{ $nodename$: `WriteData`, codedata: ethers.utils.hexlify(0x5f + tagsize) }, { $nodename$: `WritePc`, tag: tag, codedata: ethers.utils.hexZeroPad('0x', tagsize) }] });
const AssemblyPlaceTag = ({ $ancestor$, $children$, tag }) => ({ $nodename$: `WriteData`, codedata: `0x5b`, tag: tag });
const AssemblyPadSection = ({ $ancestor$, $children$, padsize }) => ({ $nodename$: `WritePad`, padsize: padsize ?? 0, $children$: $children$ });
const AssemblySection = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, $children$: $children$ });
const AssemblyPUSH = ({ $ancestor$, $children$, pushdata }) => ({ $nodename$: `WriteData`, $children$: [{ $nodename$: `WriteData`, codedata: ethers.utils.hexlify(0x5f + ethers.utils.hexDataLength(pushdata)) }, { $nodename$: `WriteData`, codedata: ethers.utils.hexlify(pushdata) }] });
const AssemblySTOP = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x00` });
const AssemblyADD = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x01` });
const AssemblyMUL = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x02` });
const AssemblySUB = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x03` });
const AssemblyDIV = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x04` });
const AssemblySDIV = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x05` });
const AssemblyMOD = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x06` });
const AssemblySMOD = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x07` });
const AssemblyADDMOD = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x08` });
const AssemblyMULMOD = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x09` });
const AssemblyEXP = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x0a` });
const AssemblySIGNEXTEND = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x0b` });
const AssemblyLT = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x10` });
const AssemblyGT = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x11` });
const AssemblySLT = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x12` });
const AssemblySGT = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x13` });
const AssemblyEQ = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x14` });
const AssemblyISZERO = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x15` });
const AssemblyAND = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x16` });
const AssemblyOR = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x17` });
const AssemblyXOR = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x18` });
const AssemblyNOT = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x19` });
const AssemblyBYTE = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x1a` });
const AssemblySHL = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x1b` });
const AssemblySHR = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x1c` });
const AssemblySAR = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x1d` });
const AssemblySHA3 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x20` });
const AssemblyADDRESS = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x30` });
const AssemblyBALANCE = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x31` });
const AssemblyORIGIN = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x32` });
const AssemblyCALLER = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x33` });
const AssemblyCALLVALUE = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x34` });
const AssemblyCALLDATALOAD = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x35` });
const AssemblyCALLDATASIZE = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x36` });
const AssemblyCALLDATACOPY = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x37` });
const AssemblyCODESIZE = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x38` });
const AssemblyCODECOPY = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x39` });
const AssemblyGASPRICE = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x3a` });
const AssemblyEXTCODESIZE = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x3b` });
const AssemblyEXTCODECOPY = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x3c` });
const AssemblyRETURNDATASIZE = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x3d` });
const AssemblyRETURNDATACOPY = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x3e` });
const AssemblyEXTCODEHASH = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x3f` });
const AssemblyBLOCKHASH = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x40` });
const AssemblyCOINBASE = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x41` });
const AssemblyTIMESTAMP = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x42` });
const AssemblyNUMBER = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x43` });
const AssemblyDIFFICULTY = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x44` });
const AssemblyGASLIMIT = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x45` });
const AssemblyCHAINID = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x46` });
const AssemblySELFBALANCE = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x47` });
const AssemblyBASEFEE = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x48` });
const AssemblyPOP = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x50` });
const AssemblyMLOAD = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x51` });
const AssemblyMSTORE = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x52` });
const AssemblyMSTORE8 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x53` });
const AssemblySLOAD = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x54` });
const AssemblySSTORE = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x55` });
const AssemblyJUMP = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x56` });
const AssemblyJUMPI = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x57` });
const AssemblyPC = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x58` });
const AssemblyMSIZE = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x59` });
const AssemblyGAS = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x5a` });
const AssemblyJUMPDEST = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x5b` });
const AssemblyDUP1 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x80` });
const AssemblyDUP2 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x81` });
const AssemblyDUP3 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x82` });
const AssemblyDUP4 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x83` });
const AssemblyDUP5 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x84` });
const AssemblyDUP6 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x85` });
const AssemblyDUP7 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x86` });
const AssemblyDUP8 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x87` });
const AssemblyDUP9 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x88` });
const AssemblyDUP10 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x89` });
const AssemblyDUP11 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x8a` });
const AssemblyDUP12 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x8b` });
const AssemblyDUP13 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x8c` });
const AssemblyDUP14 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x8d` });
const AssemblyDUP15 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x8e` });
const AssemblyDUP16 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x8f` });
const AssemblySWAP1 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x90` });
const AssemblySWAP2 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x91` });
const AssemblySWAP3 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x92` });
const AssemblySWAP4 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x93` });
const AssemblySWAP5 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x94` });
const AssemblySWAP6 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x95` });
const AssemblySWAP7 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x96` });
const AssemblySWAP8 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x97` });
const AssemblySWAP9 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x98` });
const AssemblySWAP10 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x99` });
const AssemblySWAP11 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x9a` });
const AssemblySWAP12 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x9b` });
const AssemblySWAP13 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x9c` });
const AssemblySWAP14 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x9d` });
const AssemblySWAP15 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x9e` });
const AssemblySWAP16 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0x9f` });
const AssemblyLOG0 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0xa0` });
const AssemblyLOG1 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0xa1` });
const AssemblyLOG2 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0xa2` });
const AssemblyLOG3 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0xa3` });
const AssemblyLOG4 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0xa4` });
const AssemblyCREATE = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0xf0` });
const AssemblyCALL = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0xf1` });
const AssemblyCALLCODE = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0xf2` });
const AssemblyRETURN = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0xf3` });
const AssemblyDELEGATECALL = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0xf4` });
const AssemblyCREATE2 = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0xf5` });
const AssemblySTATICCALL = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0xfa` });
const AssemblyREVERT = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0xfd` });
const AssemblySELFDESTRUCT = ({ $ancestor$, $children$ }) => ({ $nodename$: `WriteData`, codedata: `0xff` });
const WritePad = ({ $ancestor$, $children$, padsize }) => ({ $nodename$: `WriteData`, $children$: [...$children$, { $nodename$: `WriteData`, codedata: ethers.utils.hexZeroPad('0x', $children$.map(getleaves).flat().reduce((a, v) => a > ethers.utils.hexDataLength(v.codedata) ? a - ethers.utils.hexDataLength(v.codedata) : 0, padsize)) }] });
const WritePc = ({ $ancestor$, $children$, codedata, tag }, ref) => ({ $nodename$: `WriteData`, codedata: getleaves(getroooot(ref)).reduce((a, v) => typeof a === 'number' ? v.$nodename$ === `WriteData` && v.tag === tag ? padnumlen(a, ethers.utils.hexDataLength(codedata)) : a + ethers.utils.hexDataLength(v.codedata ?? "0x") : a, 0) });
const WriteData = ({ $ancestor$, $children$, codedata, tag }) => codedata ? { codedata } : { codedata: ethers.utils.hexConcat($children$.map(v => WriteData(v).codedata)) };

const compilers = [
    BasicExpressionVarSwap,
    BasicExpressionVarSet,
    BasicExpressionVar,
    BasicStatementWhile,
    BasicStatementIf,
    BasicStatementUnless,
    BasicStatementDowhile,
    BasicStatementScope,
    BasicStatementSection,
    BasicStatementStacktopRightADD,
    BasicStatementStacktopRightMUL,
    BasicStatementStacktopRightSUB,
    BasicStatementStacktopRightDIV,
    BasicStatementStacktopRightSDIV,
    BasicStatementStacktopRightMOD,
    BasicStatementStacktopRightSMOD,
    BasicStatementStacktopRightEXP,
    BasicStatementStacktopRightSIGNEXTEND,
    BasicStatementStacktopRightLT,
    BasicStatementStacktopRightGT,
    BasicStatementStacktopRightSLT,
    BasicStatementStacktopRightSGT,
    BasicStatementStacktopRightEQ,
    BasicStatementStacktopRightAND,
    BasicStatementStacktopRightOR,
    BasicStatementStacktopRightXOR,
    BasicStatementStacktopRightBYTE,
    BasicStatementStacktopRightSHL,
    BasicStatementStacktopRightSHR,
    BasicStatementStacktopRightSAR,
    BasicStatementStacktopRightSHA3,
    BasicStatementStacktopLeftADD,
    BasicStatementStacktopLeftMUL,
    BasicStatementStacktopLeftSUB,
    BasicStatementStacktopLeftDIV,
    BasicStatementStacktopLeftSDIV,
    BasicStatementStacktopLeftMOD,
    BasicStatementStacktopLeftSMOD,
    BasicStatementStacktopLeftEXP,
    BasicStatementStacktopLeftSIGNEXTEND,
    BasicStatementStacktopLeftLT,
    BasicStatementStacktopLeftGT,
    BasicStatementStacktopLeftSLT,
    BasicStatementStacktopLeftSGT,
    BasicStatementStacktopLeftEQ,
    BasicStatementStacktopLeftAND,
    BasicStatementStacktopLeftOR,
    BasicStatementStacktopLeftXOR,
    BasicStatementStacktopLeftBYTE,
    BasicStatementStacktopLeftSHL,
    BasicStatementStacktopLeftSHR,
    BasicStatementStacktopLeftSAR,
    BasicStatementStacktopLeftSHA3,
    BasicStatementStacktopISZERO,
    BasicStatementStacktopNOT,
    BasicStatementStacktopBALANCE,
    BasicStatementStacktopCALLDATALOAD,
    BasicStatementStacktopEXTCODESIZE,
    BasicStatementStacktopEXTCODEHASH,
    BasicStatementStacktopBLOCKHASH,
    BasicStatementStacktopMLOAD,
    BasicStatementStacktopSLOAD,
    BasicStatementStackSwap,
    BasicStatementStackSet,
    BasicStatementMemorySetByte,
    BasicStatementMemorySet,
    BasicStatementStorageSet,
    BasicStatementCalldataCopy,
    BasicStatementReturndataCopy,
    BasicStatementCodeCopy,
    BasicStatementExtcodeCopy,
    BasicStatementDrop,
    BasicStatementLog,
    BasicStatementSelfDestruct,
    BasicStatementRevert,
    BasicStatementReturn,
    BasicStatementStop,
    BasicExpressionStack,
    BasicExpressionPlaceholder,
    BasicExpressionLiteral,
    BasicExpressionADD,
    BasicExpressionMUL,
    BasicExpressionSUB,
    BasicExpressionDIV,
    BasicExpressionSDIV,
    BasicExpressionMOD,
    BasicExpressionSMOD,
    BasicExpressionEXP,
    BasicExpressionSIGNEXTEND,
    BasicExpressionLT,
    BasicExpressionGT,
    BasicExpressionSLT,
    BasicExpressionSGT,
    BasicExpressionEQ,
    BasicExpressionAND,
    BasicExpressionOR,
    BasicExpressionXOR,
    BasicExpressionBYTE,
    BasicExpressionSHL,
    BasicExpressionSHR,
    BasicExpressionSAR,
    BasicExpressionSHA3,
    BasicExpressionADDMOD,
    BasicExpressionMULMOD,
    BasicExpressionCREATE,
    BasicExpressionCREATE2,
    BasicExpressionDELEGATECALL,
    BasicExpressionSTATICCALL,
    BasicExpressionCALL,
    BasicExpressionCALLCODE,
    BasicExpressionISZERO,
    BasicExpressionNOT,
    BasicExpressionBALANCE,
    BasicExpressionCALLDATALOAD,
    BasicExpressionEXTCODESIZE,
    BasicExpressionEXTCODEHASH,
    BasicExpressionBLOCKHASH,
    BasicExpressionMLOAD,
    BasicExpressionSLOAD,
    BasicExpressionADDRESS,
    BasicExpressionORIGIN,
    BasicExpressionCALLER,
    BasicExpressionCALLVALUE,
    BasicExpressionCALLDATASIZE,
    BasicExpressionCODESIZE,
    BasicExpressionGASPRICE,
    BasicExpressionRETURNDATASIZE,
    BasicExpressionCODEBASE,
    BasicExpressionTIMESTAMP,
    BasicExpressionNUMBER,
    BasicExpressionDIFFICULTY,
    BasicExpressionGASLIMIT,
    BasicExpressionCHAINID,
    BasicExpressionSELFBALANCE,
    BasicExpressionBASEFEE,
    BasicExpressionPC,
    BasicExpressionMSIZE,
    BasicExpressionGAS,
    BasicExpressionStorage,
    BasicExpressionMemory,
    BasicExpressionCalldata,
    BasicClean,
    AssemblyJumpIf,
    AssemblyJumpTo,
    AssemblyPushTag,
    AssemblyPlaceTag,
    AssemblyPadSection,
    AssemblySection,
    AssemblyPUSH,
    AssemblySTOP,
    AssemblyADD,
    AssemblyMUL,
    AssemblySUB,
    AssemblyDIV,
    AssemblySDIV,
    AssemblyMOD,
    AssemblySMOD,
    AssemblyADDMOD,
    AssemblyMULMOD,
    AssemblyEXP,
    AssemblySIGNEXTEND,
    AssemblyLT,
    AssemblyGT,
    AssemblySLT,
    AssemblySGT,
    AssemblyEQ,
    AssemblyISZERO,
    AssemblyAND,
    AssemblyOR,
    AssemblyXOR,
    AssemblyNOT,
    AssemblyBYTE,
    AssemblySHL,
    AssemblySHR,
    AssemblySAR,
    AssemblySHA3,
    AssemblyADDRESS,
    AssemblyBALANCE,
    AssemblyORIGIN,
    AssemblyCALLER,
    AssemblyCALLVALUE,
    AssemblyCALLDATALOAD,
    AssemblyCALLDATASIZE,
    AssemblyCALLDATACOPY,
    AssemblyCODESIZE,
    AssemblyCODECOPY,
    AssemblyGASPRICE,
    AssemblyEXTCODESIZE,
    AssemblyEXTCODECOPY,
    AssemblyRETURNDATASIZE,
    AssemblyRETURNDATACOPY,
    AssemblyEXTCODEHASH,
    AssemblyBLOCKHASH,
    AssemblyCOINBASE,
    AssemblyTIMESTAMP,
    AssemblyNUMBER,
    AssemblyDIFFICULTY,
    AssemblyGASLIMIT,
    AssemblyCHAINID,
    AssemblySELFBALANCE,
    AssemblyBASEFEE,
    AssemblyPOP,
    AssemblyMLOAD,
    AssemblyMSTORE,
    AssemblyMSTORE8,
    AssemblySLOAD,
    AssemblySSTORE,
    AssemblyJUMP,
    AssemblyJUMPI,
    AssemblyPC,
    AssemblyMSIZE,
    AssemblyGAS,
    AssemblyJUMPDEST,
    AssemblyDUP1,
    AssemblyDUP2,
    AssemblyDUP3,
    AssemblyDUP4,
    AssemblyDUP5,
    AssemblyDUP6,
    AssemblyDUP7,
    AssemblyDUP8,
    AssemblyDUP9,
    AssemblyDUP10,
    AssemblyDUP11,
    AssemblyDUP12,
    AssemblyDUP13,
    AssemblyDUP14,
    AssemblyDUP15,
    AssemblyDUP16,
    AssemblySWAP1,
    AssemblySWAP2,
    AssemblySWAP3,
    AssemblySWAP4,
    AssemblySWAP5,
    AssemblySWAP6,
    AssemblySWAP7,
    AssemblySWAP8,
    AssemblySWAP9,
    AssemblySWAP10,
    AssemblySWAP11,
    AssemblySWAP12,
    AssemblySWAP13,
    AssemblySWAP14,
    AssemblySWAP15,
    AssemblySWAP16,
    AssemblyLOG0,
    AssemblyLOG1,
    AssemblyLOG2,
    AssemblyLOG3,
    AssemblyLOG4,
    AssemblyCREATE,
    AssemblyCALL,
    AssemblyCALLCODE,
    AssemblyRETURN,
    AssemblyDELEGATECALL,
    AssemblyCREATE2,
    AssemblySTATICCALL,
    AssemblyREVERT,
    AssemblySELFDESTRUCT,
    WritePad,
    WritePc,
    WriteData,
];

module.exports = root => compilers.reduce((a, v) => recursion(v)(syncnodes(a)), root).codedata;

if (require.main === module) {
    console.log(module.exports(JSON.parse(require(`fs`).readFileSync(0, 'utf-8'))));
}
