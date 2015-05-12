# echo "setup rcd_trb HEAD in /afs/cern.ch/user/d/daquser/public/ATLAS_DAQ/DataFlow"

if test "${CMTROOT}" = ""; then
  CMTROOT=/afs/cern.ch/sw/contrib/CMT/v1r25p20130606; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtrcd_trbtempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtrcd_trbtempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=rcd_trb -version=HEAD -path=/afs/cern.ch/user/d/daquser/public/ATLAS_DAQ/DataFlow  -no_cleanup $* >${cmtrcd_trbtempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt setup -sh -pack=rcd_trb -version=HEAD -path=/afs/cern.ch/user/d/daquser/public/ATLAS_DAQ/DataFlow  -no_cleanup $* >${cmtrcd_trbtempfile}"
  cmtsetupstatus=2
  /bin/rm -f ${cmtrcd_trbtempfile}
  unset cmtrcd_trbtempfile
  return $cmtsetupstatus
fi
cmtsetupstatus=0
. ${cmtrcd_trbtempfile}
if test $? != 0 ; then
  cmtsetupstatus=2
fi
/bin/rm -f ${cmtrcd_trbtempfile}
unset cmtrcd_trbtempfile
return $cmtsetupstatus
