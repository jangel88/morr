nodeid_validity valid_nodeid(nodeid nid)
{
  if  (nid>-1 && nid<19200){
    return VALID;
  } else {
    return INVALID;
  }
}

nodeid_validity valid_node_list(int N, *nodeid node_list)
/* Check that node_list of length N contains only 
 * valid node ids*/
{
  int i;
  for(i=0;i<N;i++){
    if (valid_nodeid(node_list[i])!=VALID){
      return INVALID;
    }
  } 
  return VALID; 
}
