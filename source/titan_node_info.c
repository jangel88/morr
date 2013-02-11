nodeid_validity valid_nodeid(int nodeid)
{
  if  (nodeid>-1 && nodeid<19200){
    return VALID;
  } else {
    return INVALID;
  }
}

nodeid_validity valid_node_list(int N, int* node_list)
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
