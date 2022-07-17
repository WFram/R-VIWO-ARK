/**
 * This file is part of ORB-SLAM3
 *
 * Copyright (C) 2017-2021 Carlos Campos, Richard Elvira, Juan J. Gómez
 * Rodríguez, José M.M. Montiel and Juan D. Tardós, University of Zaragoza.
 * Copyright (C) 2014-2016 Raúl Mur-Artal, José M.M. Montiel and Juan D. Tardós,
 * University of Zaragoza.
 *
 * ORB-SLAM3 is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * ORB-SLAM3 is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ORB-SLAM3. If not, see <http://www.gnu.org/licenses/>.
 */

#include "KeyFrame.h"
#include <mutex>
#include "Converter.h"
#include "ImuTypes.h"

namespace ORB_SLAM3 {

long unsigned int KeyFrame::nNextId = 0;

KeyFrame::KeyFrame()
    : mnFrameId(0),
      mTimeStamp(0),
      mnGridCols(FRAME_GRID_COLS),
      mnGridRows(FRAME_GRID_ROWS),
      mfGridElementWidthInv(0),
      mfGridElementHeightInv(0),
      mnTrackReferenceForFrame(0),
      mnFuseTargetForKF(0),
      mnBALocalForKF(0),
      mnBAFixedForKF(0),
      mnBALocalForMerge(0),
      mnLoopQuery(0),
      mnLoopWords(0),
      mnRelocQuery(0),
      mnRelocWords(0),
      mnMergeQuery(0),
      mnMergeWords(0),
      mnBAGlobalForKF(0),
      fx(0),
      fy(0),
      cx(0),
      cy(0),
      invfx(0),
      invfy(0),
      mnPlaceRecognitionQuery(0),
      mnPlaceRecognitionWords(0),
      mPlaceRecognitionScore(0),
      mbf(0),
      mb(0),
      mThDepth(0),
      N(0),
      mvKeys(static_cast<vector<cv::KeyPoint>>(0)),
      mvKeysUn(static_cast<vector<cv::KeyPoint>>(0)),
      mvSegVal(static_cast<float>(0.0f)),
      mbIsPropagated(static_cast<vector<bool>>(false)),
      mvProb(static_cast<vector<double>>(0)),
      mvSegDynKeys(static_cast<vector<cv::KeyPoint>>(NULL)),
      mvuRight(static_cast<vector<float>>(0)),
      mvDepth(static_cast<vector<float>>(0)),
      mnScaleLevels(0),
      mfScaleFactor(0),
      mfLogScaleFactor(0),
      mvScaleFactors(0),
      mvLevelSigma2(0),
      mvInvLevelSigma2(0),
      mnMinX(0),
      mnMinY(0),
      mnMaxX(0),
      mnMaxY(0),
      mPrevKF(static_cast<KeyFrame *>(nullptr)),
      mNextKF(static_cast<KeyFrame *>(nullptr)),
      mbFirstConnection(true),
      mpParent(nullptr),
      mbNotErase(false),
      mbToBeErased(false),
      mbBad(false),
      mHalfBaseline(0),
      mbCurrentPlaceRecognition(false),
      mnMergeCorrectedForKF(0),
      // depth_(static_cast<cv::Mat*>(NULL)),
      // gray_(static_cast<cv::Mat*>(NULL)),
      NLeft(0),
      NRight(0),
      mnNumberOfOpt(0),
      mbHasVelocity(false) {}

KeyFrame::KeyFrame(Frame &F, Map *pMap, KeyFrameDatabase *pKFDB)
    : bImu(pMap->isImuInitialized()),
      mnFrameId(F.mnId),
      mTimeStamp(F.mTimeStamp),
      mnGridCols(FRAME_GRID_COLS),
      mnGridRows(FRAME_GRID_ROWS),
      mfGridElementWidthInv(F.mfGridElementWidthInv),
      mfGridElementHeightInv(F.mfGridElementHeightInv),
      mnTrackReferenceForFrame(0),
      mnFuseTargetForKF(0),
      mnBALocalForKF(0),
      mnBAFixedForKF(0),
      mnBALocalForMerge(0),
      mnLoopQuery(0),
      mnLoopWords(0),
      mnRelocQuery(0),
      mnRelocWords(0),
      mnBAGlobalForKF(0),
      mnPlaceRecognitionQuery(0),
      mnPlaceRecognitionWords(0),
      mPlaceRecognitionScore(0),
      fx(F.fx),
      fy(F.fy),
      cx(F.cx),
      cy(F.cy),
      invfx(F.invfx),
      invfy(F.invfy),
      mbf(F.mbf),
      mb(F.mb),
      mThDepth(F.mThDepth),
      N(F.N),
      mvKeys(F.mvKeys),
      mvKeysUn(F.mvKeysUn),
      mvSegVal(F.mvSegVal),
      mbIsPropagated(F.mbIsPropagated),
      mvProb(F.mvProb),
      mvSegDynKeys(F.mvDynKeys),
      mvuRight(F.mvuRight),
      mvDepth(F.mvDepth),
      mDescriptors(F.mDescriptors.clone()),
      mBowVec(F.mBowVec),
      mFeatVec(F.mFeatVec),
      mnScaleLevels(F.mnScaleLevels),
      mfScaleFactor(F.mfScaleFactor),
      mfLogScaleFactor(F.mfLogScaleFactor),
      mvScaleFactors(F.mvScaleFactors),
      mvLevelSigma2(F.mvLevelSigma2),
      mvInvLevelSigma2(F.mvInvLevelSigma2),
      mnMinX(F.mnMinX),
      mnMinY(F.mnMinY),
      mnMaxX(F.mnMaxX),
      mnMaxY(F.mnMaxY),
      mK_(F.mK_),
      mPrevKF(nullptr),
      mNextKF(nullptr),
      mpImuPreintegrated(F.mpImuPreintegrated),
      mImuCalib(F.mImuCalib),
      mpOdomPreintegrated(F.mpOdomPreintegrated),
      mvpMapPoints(F.mvpMapPoints),
      mpKeyFrameDB(pKFDB),
      mpORBvocabulary(F.mpORBvocabulary),
      mbFirstConnection(true),
      mpParent(nullptr),
      mDistCoef(F.mDistCoef),
      mbNotErase(false),
      mnDataset(F.mnDataset),
      imgLeft(F.imgLeft.clone()),
      mbToBeErased(false),
      mbBad(false),
      mHalfBaseline(F.mb / 2),
      mpMap(pMap),
      mbCurrentPlaceRecognition(false),
      mNameFile(F.mNameFile),
      mnMergeCorrectedForKF(0),
      depth_(F.depth_.clone()),
      gray_(F.gray_.clone()),
      mpCamera(F.mpCamera),
      mpCamera2(F.mpCamera2),
      mvLeftToRightMatch(F.mvLeftToRightMatch),
      mvRightToLeftMatch(F.mvRightToLeftMatch),
      mTlr(F.GetRelativePoseTlr()),
      mvKeysRight(F.mvKeysRight),
      NLeft(F.Nleft),
      NRight(F.Nright),
      mTrl(F.GetRelativePoseTrl()),
      mnNumberOfOpt(0),
      mbHasVelocity(false) {
  mnId = nNextId++;

  mGrid.resize(mnGridCols);
  if (F.Nleft != -1) mGridRight.resize(mnGridCols);
  for (int i = 0; i < mnGridCols; i++) {
    mGrid[i].resize(mnGridRows);
    if (F.Nleft != -1) mGridRight[i].resize(mnGridRows);
    for (int j = 0; j < mnGridRows; j++) {
      mGrid[i][j] = F.mGrid[i][j];
      if (F.Nleft != -1) {
        mGridRight[i][j] = F.mGridRight[i][j];
      }
    }
  }

  if (!F.HasVelocity()) {
    mVw.setZero();
    mbHasVelocity = false;
  } else {
    mVw = F.GetVelocity();
    mbHasVelocity = true;
  }

  mImuBias = F.mImuBias;
  SetPose(F.GetPose());

  mnOriginMapId = pMap->GetId();
}

void KeyFrame::ComputeBoW() {
  if (mBowVec.empty() || mFeatVec.empty()) {
    vector<cv::Mat> vCurrentDesc = Converter::toDescriptorVector(mDescriptors);
    // Feature vector associate features with nodes in the 4th level (from
    // leaves up) We assume the vocabulary tree has 6 levels, change the 4
    // otherwise
    mpORBvocabulary->transform(vCurrentDesc, mBowVec, mFeatVec, 4);
  }
}

void KeyFrame::SetPose(const Sophus::SE3f &Tcw) {
  unique_lock<mutex> lock(mMutexPose);

  mTcw = Tcw;
  mRcw = mTcw.rotationMatrix();
  mTwc = mTcw.inverse();
  mRwc = mTwc.rotationMatrix();

  if (mImuCalib.mbIsSet)  // TODO Use a flag instead of the OpenCV matrix
  {
    mOwb = mRwc * mImuCalib.mTcb.translation() + mTwc.translation();
  }
}

void KeyFrame::SetVelocity(const Eigen::Vector3f &Vw) {
  unique_lock<mutex> lock(mMutexPose);
  mVw = Vw;
  mbHasVelocity = true;
}

Sophus::SE3f KeyFrame::GetPose() {
  unique_lock<mutex> lock(mMutexPose);
  return mTcw;
}

Sophus::SE3f KeyFrame::GetPoseInverse() {
  unique_lock<mutex> lock(mMutexPose);
  return mTwc;
}

Eigen::Vector3f KeyFrame::GetCameraCenter() {
  unique_lock<mutex> lock(mMutexPose);
  return mTwc.translation();
}

Eigen::Vector3f KeyFrame::GetImuPosition() {
  unique_lock<mutex> lock(mMutexPose);
  return mOwb;
}

Eigen::Matrix3f KeyFrame::GetImuRotation() {
  unique_lock<mutex> lock(mMutexPose);
  return (mTwc * mImuCalib.mTcb).rotationMatrix();
}

Sophus::SE3f KeyFrame::GetImuPose() {
  unique_lock<mutex> lock(mMutexPose);
  return mTwc * mImuCalib.mTcb;
}

Eigen::Matrix3f KeyFrame::GetRotation() {
  unique_lock<mutex> lock(mMutexPose);
  return mRcw;
}

Eigen::Vector3f KeyFrame::GetTranslation() {
  unique_lock<mutex> lock(mMutexPose);
  return mTcw.translation();
}

Eigen::Vector3f KeyFrame::GetVelocity() {
  unique_lock<mutex> lock(mMutexPose);
  return mVw;
}

bool KeyFrame::isVelocitySet() {
  unique_lock<mutex> lock(mMutexPose);
  return mbHasVelocity;
}

void KeyFrame::AddConnection(KeyFrame *pKF, const int &weight) {
  {
    unique_lock<mutex> lock(mMutexConnections);
    if (!mConnectedKeyFrameWeights.count(pKF) ||
        mConnectedKeyFrameWeights[pKF] != weight)
      mConnectedKeyFrameWeights[pKF] = weight;
    else
      return;
  }

  UpdateBestCovisibles();
}

void KeyFrame::UpdateBestCovisibles() {
  unique_lock<mutex> lock(mMutexConnections);
  vector<pair<int, KeyFrame *>> vPairs;
  vPairs.reserve(mConnectedKeyFrameWeights.size());
  for (auto &mConnectedKeyFrameWeight : mConnectedKeyFrameWeights)
    vPairs.push_back(make_pair(mConnectedKeyFrameWeight.second,
                               mConnectedKeyFrameWeight.first));

  sort(vPairs.begin(), vPairs.end());
  list<KeyFrame *> lKFs;
  list<int> lWs;
  for (auto &vPair : vPairs) {
    if (!vPair.second->isBad()) {
      lKFs.push_front(vPair.second);
      lWs.push_front(vPair.first);
    }
  }

  mvpOrderedConnectedKeyFrames = vector<KeyFrame *>(lKFs.begin(), lKFs.end());
  mvOrderedWeights = vector<int>(lWs.begin(), lWs.end());
}

set<KeyFrame *> KeyFrame::GetConnectedKeyFrames() {
  unique_lock<mutex> lock(mMutexConnections);
  set<KeyFrame *> s;
  for (auto &mConnectedKeyFrameWeight : mConnectedKeyFrameWeights)
    s.insert(mConnectedKeyFrameWeight.first);
  return s;
}

vector<KeyFrame *> KeyFrame::GetVectorCovisibleKeyFrames() {
  unique_lock<mutex> lock(mMutexConnections);
  return mvpOrderedConnectedKeyFrames;
}

vector<KeyFrame *> KeyFrame::GetBestCovisibilityKeyFrames(const int &N) {
  unique_lock<mutex> lock(mMutexConnections);
  return (int)mvpOrderedConnectedKeyFrames.size() < N
             ? mvpOrderedConnectedKeyFrames
             : vector<KeyFrame *>(mvpOrderedConnectedKeyFrames.begin(),
                                  mvpOrderedConnectedKeyFrames.begin() + N);
}

vector<KeyFrame *> KeyFrame::GetCovisiblesByWeight(const int &w) {
  unique_lock<mutex> lock(mMutexConnections);

  if (mvpOrderedConnectedKeyFrames.empty()) {
    return {};
  }

  auto it = upper_bound(mvOrderedWeights.begin(), mvOrderedWeights.end(), w,
                        KeyFrame::weightComp);

  if (it == mvOrderedWeights.end() && mvOrderedWeights.back() < w) {
    return {};
  } else {
    size_t n = it - mvOrderedWeights.begin();
    return vector<KeyFrame *>(mvpOrderedConnectedKeyFrames.begin(),
                              mvpOrderedConnectedKeyFrames.begin() + n);
  }
}

int KeyFrame::GetWeight(KeyFrame *pKF) {
  unique_lock<mutex> lock(mMutexConnections);
  if (mConnectedKeyFrameWeights.count(pKF))
    return mConnectedKeyFrameWeights[pKF];
  else
    return 0;
}

int KeyFrame::GetNumberMPs() {
  unique_lock<mutex> lock(mMutexFeatures);
  int numberMPs = 0;
  for (auto &mvpMapPoint : mvpMapPoints) {
    if (!mvpMapPoint) continue;
    numberMPs++;
  }
  return numberMPs;
}

void KeyFrame::AddMapPoint(MapPoint *pMP, const size_t &idx) {
  unique_lock<mutex> lock(mMutexFeatures);
  mvpMapPoints[idx] = pMP;
}

void KeyFrame::EraseMapPointMatch(const int &idx) {
  unique_lock<mutex> lock(mMutexFeatures);
  mvpMapPoints[idx] = static_cast<MapPoint *>(nullptr);
}

void KeyFrame::EraseMapPointMatch(MapPoint *pMP) {
  tuple<size_t, size_t> indexes = pMP->GetIndexInKeyFrame(this);
  size_t leftIndex = get<0>(indexes), rightIndex = get<1>(indexes);
  if (leftIndex != -1)
    mvpMapPoints[leftIndex] = static_cast<MapPoint *>(nullptr);
  if (rightIndex != -1)
    mvpMapPoints[rightIndex] = static_cast<MapPoint *>(nullptr);
}

void KeyFrame::ReplaceMapPointMatch(const int &idx, MapPoint *pMP) {
  mvpMapPoints[idx] = pMP;
}

set<MapPoint *> KeyFrame::GetMapPoints() {
  unique_lock<mutex> lock(mMutexFeatures);
  set<MapPoint *> s;
  for (auto &mvpMapPoint : mvpMapPoints) {
    if (!mvpMapPoint) continue;
    MapPoint *pMP = mvpMapPoint;
    if (!pMP->isBad()) s.insert(pMP);
  }
  return s;
}

int KeyFrame::TrackedMapPoints(const int &minObs) {
  unique_lock<mutex> lock(mMutexFeatures);

  int nPoints = 0;
  const bool bCheckObs = minObs > 0;
  for (int i = 0; i < N; i++) {
    MapPoint *pMP = mvpMapPoints[i];
    if (pMP) {
      if (!pMP->isBad()) {
        if (bCheckObs) {
          if (mvpMapPoints[i]->Observations() >= minObs) nPoints++;
        } else
          nPoints++;
      }
    }
  }

  return nPoints;
}

vector<MapPoint *> KeyFrame::GetMapPointMatches() {
  unique_lock<mutex> lock(mMutexFeatures);
  return mvpMapPoints;
}

MapPoint *KeyFrame::GetMapPoint(const size_t &idx) {
  unique_lock<mutex> lock(mMutexFeatures);
  return mvpMapPoints[idx];
}

void KeyFrame::UpdateConnections(bool upParent) {
  map<KeyFrame *, int> KFcounter;

  vector<MapPoint *> vpMP;

  {
    unique_lock<mutex> lockMPs(mMutexFeatures);
    vpMP = mvpMapPoints;
  }

  // For all map points in keyframe check in which other keyframes are they seen
  // Increase counter for those keyframes
  for (auto pMP : vpMP) {
    if (!pMP) continue;

    if (pMP->isBad()) continue;

    map<KeyFrame *, tuple<int, int>> observations = pMP->GetObservations();

    for (auto &observation : observations) {
      if (observation.first->mnId == mnId || observation.first->isBad() ||
          observation.first->GetMap() != mpMap)
        continue;
      KFcounter[observation.first]++;
    }
  }

  // This should not happen
  if (KFcounter.empty()) return;

  // If the counter is greater than threshold add connection
  // In case no keyframe counter is over threshold add the one with maximum
  // counter
  int nmax = 0;
  KeyFrame *pKFmax = nullptr;
  int th = 15;

  vector<pair<int, KeyFrame *>> vPairs;
  vPairs.reserve(KFcounter.size());
  if (!upParent) cout << "UPDATE_CONN: current KF " << mnId << endl;
  for (auto &mit : KFcounter) {
    if (!upParent)
      cout << "  UPDATE_CONN: KF " << mit.first->mnId
           << " ; num matches: " << mit.second << endl;
    if (mit.second > nmax) {
      nmax = mit.second;
      pKFmax = mit.first;
    }
    if (mit.second >= th) {
      vPairs.push_back(make_pair(mit.second, mit.first));
      (mit.first)->AddConnection(this, mit.second);
    }
  }

  if (vPairs.empty()) {
    vPairs.push_back(make_pair(nmax, pKFmax));
    pKFmax->AddConnection(this, nmax);
  }

  sort(vPairs.begin(), vPairs.end());
  list<KeyFrame *> lKFs;
  list<int> lWs;
  for (auto &vPair : vPairs) {
    lKFs.push_front(vPair.second);
    lWs.push_front(vPair.first);
  }

  {
    unique_lock<mutex> lockCon(mMutexConnections);

    mConnectedKeyFrameWeights = KFcounter;
    mvpOrderedConnectedKeyFrames = vector<KeyFrame *>(lKFs.begin(), lKFs.end());
    mvOrderedWeights = vector<int>(lWs.begin(), lWs.end());

    if (mbFirstConnection && mnId != mpMap->GetInitKFid()) {
      mpParent = mvpOrderedConnectedKeyFrames.front();
      mpParent->AddChild(this);
      mbFirstConnection = false;
    }
  }
}

void KeyFrame::AddChild(KeyFrame *pKF) {
  unique_lock<mutex> lockCon(mMutexConnections);
  mspChildrens.insert(pKF);
}

void KeyFrame::EraseChild(KeyFrame *pKF) {
  unique_lock<mutex> lockCon(mMutexConnections);
  mspChildrens.erase(pKF);
}

void KeyFrame::ChangeParent(KeyFrame *pKF) {
  unique_lock<mutex> lockCon(mMutexConnections);
  if (pKF == this) {
    cout << "ERROR: Change parent KF, the parent and child are the same KF"
         << endl;
    throw std::invalid_argument("The parent and child can not be the same");
  }
  if (pKF) {
    mpParent = pKF;
    pKF->AddChild(this);
  }
}

set<KeyFrame *> KeyFrame::GetChilds() {
  unique_lock<mutex> lockCon(mMutexConnections);
  return mspChildrens;
}

KeyFrame *KeyFrame::GetParent() {
  unique_lock<mutex> lockCon(mMutexConnections);
  return mpParent;
}

bool KeyFrame::hasChild(KeyFrame *pKF) {
  unique_lock<mutex> lockCon(mMutexConnections);
  return mspChildrens.count(pKF);
}

void KeyFrame::SetFirstConnection(bool bFirst) {
  unique_lock<mutex> lockCon(mMutexConnections);
  mbFirstConnection = bFirst;
}

void KeyFrame::AddLoopEdge(KeyFrame *pKF) {
  unique_lock<mutex> lockCon(mMutexConnections);
  mbNotErase = true;
  mspLoopEdges.insert(pKF);
}

set<KeyFrame *> KeyFrame::GetLoopEdges() {
  unique_lock<mutex> lockCon(mMutexConnections);
  return mspLoopEdges;
}

void KeyFrame::AddMergeEdge(KeyFrame *pKF) {
  unique_lock<mutex> lockCon(mMutexConnections);
  mbNotErase = true;
  mspMergeEdges.insert(pKF);
}

set<KeyFrame *> KeyFrame::GetMergeEdges() {
  unique_lock<mutex> lockCon(mMutexConnections);
  return mspMergeEdges;
}

void KeyFrame::SetNotErase() {
  unique_lock<mutex> lock(mMutexConnections);
  mbNotErase = true;
}

void KeyFrame::SetErase() {
  {
    unique_lock<mutex> lock(mMutexConnections);
    if (mspLoopEdges.empty()) {
      mbNotErase = false;
    }
  }

  if (mbToBeErased) {
    SetBadFlag();
  }
}

void KeyFrame::SetBadFlag() {
  {
    unique_lock<mutex> lock(mMutexConnections);
    if (mnId == mpMap->GetInitKFid()) {
      return;
    } else if (mbNotErase) {
      mbToBeErased = true;
      return;
    }
  }

  for (auto &mConnectedKeyFrameWeight : mConnectedKeyFrameWeights) {
    mConnectedKeyFrameWeight.first->EraseConnection(this);
  }

  for (auto &mvpMapPoint : mvpMapPoints) {
    if (mvpMapPoint) {
      mvpMapPoint->EraseObservation(this);
    }
  }

  {
    unique_lock<mutex> lock(mMutexConnections);
    unique_lock<mutex> lock1(mMutexFeatures);

    mConnectedKeyFrameWeights.clear();
    mvpOrderedConnectedKeyFrames.clear();

    // Update Spanning Tree
    set<KeyFrame *> sParentCandidates;
    if (mpParent) sParentCandidates.insert(mpParent);

    // Assign at each iteration one children with a parent (the pair with
    // highest covisibility weight) Include that children as new parent
    // candidate for the rest
    while (!mspChildrens.empty()) {
      bool bContinue = false;

      int max = -1;
      KeyFrame *pC;
      KeyFrame *pP;

      for (auto pKF : mspChildrens) {
        if (pKF->isBad()) continue;

        // Check if a parent candidate is connected to the keyframe
        vector<KeyFrame *> vpConnected = pKF->GetVectorCovisibleKeyFrames();
        for (auto &i : vpConnected) {
          for (auto sParentCandidate : sParentCandidates) {
            if (i->mnId == sParentCandidate->mnId) {
              int w = pKF->GetWeight(i);
              if (w > max) {
                pC = pKF;
                pP = i;
                max = w;
                bContinue = true;
              }
            }
          }
        }
      }

      if (bContinue) {
        pC->ChangeParent(pP);
        sParentCandidates.insert(pC);
        mspChildrens.erase(pC);
      } else
        break;
    }

    // If a children has no covisibility links with any parent candidate, assign
    // to the original parent of this KF
    if (!mspChildrens.empty()) {
      for (auto mspChildren : mspChildrens) {
        mspChildren->ChangeParent(mpParent);
      }
    }

    if (mpParent) {
      mpParent->EraseChild(this);
      mTcp = mTcw * mpParent->GetPoseInverse();
    }
    mbBad = true;
  }

  mpMap->EraseKeyFrame(this);
  mpKeyFrameDB->erase(this);
}

bool KeyFrame::isBad() {
  unique_lock<mutex> lock(mMutexConnections);
  return mbBad;
}

void KeyFrame::EraseConnection(KeyFrame *pKF) {
  bool bUpdate = false;
  {
    unique_lock<mutex> lock(mMutexConnections);
    if (mConnectedKeyFrameWeights.count(pKF)) {
      mConnectedKeyFrameWeights.erase(pKF);
      bUpdate = true;
    }
  }

  if (bUpdate) UpdateBestCovisibles();
}

vector<size_t> KeyFrame::GetFeaturesInArea(const float &x, const float &y,
                                           const float &r,
                                           const bool bRight) const {
  vector<size_t> vIndices;
  vIndices.reserve(N);

  float factorX = r;
  float factorY = r;

  const int nMinCellX =
      max(0, (int)floor((x - mnMinX - factorX) * mfGridElementWidthInv));
  if (nMinCellX >= mnGridCols) return vIndices;

  const int nMaxCellX =
      min((int)mnGridCols - 1,
          (int)ceil((x - mnMinX + factorX) * mfGridElementWidthInv));
  if (nMaxCellX < 0) return vIndices;

  const int nMinCellY =
      max(0, (int)floor((y - mnMinY - factorY) * mfGridElementHeightInv));
  if (nMinCellY >= mnGridRows) return vIndices;

  const int nMaxCellY =
      min((int)mnGridRows - 1,
          (int)ceil((y - mnMinY + factorY) * mfGridElementHeightInv));
  if (nMaxCellY < 0) return vIndices;

  for (int ix = nMinCellX; ix <= nMaxCellX; ix++) {
    for (int iy = nMinCellY; iy <= nMaxCellY; iy++) {
      const vector<size_t> vCell =
          (!bRight) ? mGrid[ix][iy] : mGridRight[ix][iy];
      for (unsigned long j : vCell) {
        const cv::KeyPoint &kpUn = (NLeft == -1)
                                       ? mvKeysUn[j]
                                       : (!bRight) ? mvKeys[j] : mvKeysRight[j];
        const float distx = kpUn.pt.x - x;
        const float disty = kpUn.pt.y - y;

        if (fabs(distx) < r && fabs(disty) < r) vIndices.push_back(j);
      }
    }
  }

  return vIndices;
}

bool KeyFrame::IsInImage(const float &x, const float &y) const {
  return (x >= mnMinX && x < mnMaxX && y >= mnMinY && y < mnMaxY);
}

bool KeyFrame::UnprojectStereo(int i, Eigen::Vector3f &x3D) {
  const float z = mvDepth[i];
  if (z > 0) {
    const float u = mvKeys[i].pt.x;
    const float v = mvKeys[i].pt.y;
    const float x = (u - cx) * z * invfx;
    const float y = (v - cy) * z * invfy;
    Eigen::Vector3f x3Dc(x, y, z);

    unique_lock<mutex> lock(mMutexPose);
    x3D = mRwc * x3Dc + mTwc.translation();
    return true;
  } else
    return false;
}

float KeyFrame::ComputeSceneMedianDepth(const int q) {
  if (N == 0) return -1.0;

  vector<MapPoint *> vpMapPoints;
  Eigen::Matrix3f Rcw;
  Eigen::Vector3f tcw;
  {
    unique_lock<mutex> lock(mMutexFeatures);
    unique_lock<mutex> lock2(mMutexPose);
    vpMapPoints = mvpMapPoints;
    tcw = mTcw.translation();
    Rcw = mRcw;
  }

  vector<float> vDepths;
  vDepths.reserve(N);
  Eigen::Matrix<float, 1, 3> Rcw2 = Rcw.row(2);
  float zcw = tcw(2);
  for (int i = 0; i < N; i++) {
    if (mvpMapPoints[i]) {
      MapPoint *pMP = mvpMapPoints[i];
      Eigen::Vector3f x3Dw = pMP->GetWorldPos();
      float z = Rcw2.dot(x3Dw) + zcw;
      vDepths.push_back(z);
    }
  }

  sort(vDepths.begin(), vDepths.end());

  return vDepths[(vDepths.size() - 1) / q];
}

void KeyFrame::SetNewBias(const IMU::Bias &b) {
  unique_lock<mutex> lock(mMutexPose);
  mImuBias = b;
  if (mpImuPreintegrated) mpImuPreintegrated->SetNewBias(b);
}

Eigen::Vector3f KeyFrame::GetGyroBias() {
  unique_lock<mutex> lock(mMutexPose);
  return Eigen::Vector3f(mImuBias.bwx, mImuBias.bwy, mImuBias.bwz);
}

Eigen::Vector3f KeyFrame::GetAccBias() {
  unique_lock<mutex> lock(mMutexPose);
  return Eigen::Vector3f(mImuBias.bax, mImuBias.bay, mImuBias.baz);
}

IMU::Bias KeyFrame::GetImuBias() {
  unique_lock<mutex> lock(mMutexPose);
  return mImuBias;
}

Map *KeyFrame::GetMap() {
  unique_lock<mutex> lock(mMutexMap);
  return mpMap;
}

void KeyFrame::UpdateMap(Map *pMap) {
  unique_lock<mutex> lock(mMutexMap);
  mpMap = pMap;
}

void KeyFrame::PreSave(set<KeyFrame *> &spKF, set<MapPoint *> &spMP,
                       set<GeometricCamera *> &spCam) {
  // Save the id of each MapPoint in this KF, there can be null pointer in the
  // vector
  mvBackupMapPointsId.clear();
  mvBackupMapPointsId.reserve(N);
  for (int i = 0; i < N; ++i) {
    if (mvpMapPoints[i] && spMP.find(mvpMapPoints[i]) !=
                               spMP.end())  // Checks if the element is not null
      mvBackupMapPointsId.push_back(mvpMapPoints[i]->mnId);
    else  // If the element is null his value is -1 because all the id are
          // positives
      mvBackupMapPointsId.push_back(-1);
  }
  // Save the id of each connected KF with it weight
  mBackupConnectedKeyFrameIdWeights.clear();
  for (auto mConnectedKeyFrameWeight : mConnectedKeyFrameWeights) {
    if (spKF.find(mConnectedKeyFrameWeight.first) != spKF.end())
      mBackupConnectedKeyFrameIdWeights[mConnectedKeyFrameWeight.first->mnId] =
          mConnectedKeyFrameWeight.second;
  }

  // Save the parent id
  mBackupParentId = -1;
  if (mpParent && spKF.find(mpParent) != spKF.end())
    mBackupParentId = mpParent->mnId;

  // Save the id of the childrens KF
  mvBackupChildrensId.clear();
  mvBackupChildrensId.reserve(mspChildrens.size());
  for (KeyFrame *pKFi : mspChildrens) {
    if (spKF.find(pKFi) != spKF.end())
      mvBackupChildrensId.push_back(pKFi->mnId);
  }

  // Save the id of the loop edge KF
  mvBackupLoopEdgesId.clear();
  mvBackupLoopEdgesId.reserve(mspLoopEdges.size());
  for (KeyFrame *pKFi : mspLoopEdges) {
    if (spKF.find(pKFi) != spKF.end())
      mvBackupLoopEdgesId.push_back(pKFi->mnId);
  }

  // Save the id of the merge edge KF
  mvBackupMergeEdgesId.clear();
  mvBackupMergeEdgesId.reserve(mspMergeEdges.size());
  for (KeyFrame *pKFi : mspMergeEdges) {
    if (spKF.find(pKFi) != spKF.end())
      mvBackupMergeEdgesId.push_back(pKFi->mnId);
  }

  // Camera data
  mnBackupIdCamera = -1;
  if (mpCamera && spCam.find(mpCamera) != spCam.end())
    mnBackupIdCamera = mpCamera->GetId();

  mnBackupIdCamera2 = -1;
  if (mpCamera2 && spCam.find(mpCamera2) != spCam.end())
    mnBackupIdCamera2 = mpCamera2->GetId();

  // Inertial data
  mBackupPrevKFId = -1;
  if (mPrevKF && spKF.find(mPrevKF) != spKF.end())
    mBackupPrevKFId = mPrevKF->mnId;

  mBackupNextKFId = -1;
  if (mNextKF && spKF.find(mNextKF) != spKF.end())
    mBackupNextKFId = mNextKF->mnId;

  if (mpImuPreintegrated) mBackupImuPreintegrated.CopyFrom(mpImuPreintegrated);
}

void KeyFrame::PostLoad(map<long unsigned int, KeyFrame *> &mpKFid,
                        map<long unsigned int, MapPoint *> &mpMPid,
                        map<unsigned int, GeometricCamera *> &mpCamId) {
  // Rebuild the empty variables

  // Pose
  SetPose(mTcw);

  mTrl = mTlr.inverse();

  // Reference reconstruction
  // Each MapPoint sight from this KeyFrame
  mvpMapPoints.clear();
  mvpMapPoints.resize(N);
  for (int i = 0; i < N; ++i) {
    if (mvBackupMapPointsId[i] != -1)
      mvpMapPoints[i] = mpMPid[mvBackupMapPointsId[i]];
    else
      mvpMapPoints[i] = static_cast<MapPoint *>(nullptr);
  }

  // Conected KeyFrames with him weight
  mConnectedKeyFrameWeights.clear();
  for (map<long unsigned int, int>::const_iterator
           it = mBackupConnectedKeyFrameIdWeights.begin(),
           end = mBackupConnectedKeyFrameIdWeights.end();
       it != end; ++it) {
    KeyFrame *pKFi = mpKFid[it->first];
    mConnectedKeyFrameWeights[pKFi] = it->second;
  }

  // Restore parent KeyFrame
  if (mBackupParentId >= 0) mpParent = mpKFid[mBackupParentId];

  // KeyFrame childrens
  mspChildrens.clear();
  for (vector<long unsigned int>::const_iterator
           it = mvBackupChildrensId.begin(),
           end = mvBackupChildrensId.end();
       it != end; ++it) {
    mspChildrens.insert(mpKFid[*it]);
  }

  // Loop edge KeyFrame
  mspLoopEdges.clear();
  for (vector<long unsigned int>::const_iterator
           it = mvBackupLoopEdgesId.begin(),
           end = mvBackupLoopEdgesId.end();
       it != end; ++it) {
    mspLoopEdges.insert(mpKFid[*it]);
  }

  // Merge edge KeyFrame
  mspMergeEdges.clear();
  for (vector<long unsigned int>::const_iterator
           it = mvBackupMergeEdgesId.begin(),
           end = mvBackupMergeEdgesId.end();
       it != end; ++it) {
    mspMergeEdges.insert(mpKFid[*it]);
  }

  // Camera data
  if (mnBackupIdCamera >= 0) {
    mpCamera = mpCamId[mnBackupIdCamera];
  } else {
    cout << "ERROR: There is not a main camera in KF " << mnId << endl;
  }
  if (mnBackupIdCamera2 >= 0) {
    mpCamera2 = mpCamId[mnBackupIdCamera2];
  }

  // Inertial data
  if (mBackupPrevKFId != -1) {
    mPrevKF = mpKFid[mBackupPrevKFId];
  }
  if (mBackupNextKFId != -1) {
    mNextKF = mpKFid[mBackupNextKFId];
  }
  mpImuPreintegrated = &mBackupImuPreintegrated;

  // Remove all backup container
  mvBackupMapPointsId.clear();
  mBackupConnectedKeyFrameIdWeights.clear();
  mvBackupChildrensId.clear();
  mvBackupLoopEdgesId.clear();

  UpdateBestCovisibles();
}

bool KeyFrame::ProjectPointDistort(MapPoint *pMP, cv::Point2f &kp, float &u,
                                   float &v) {
  // 3D in absolute coordinates
  Eigen::Vector3f P = pMP->GetWorldPos();

  // 3D in camera coordinates
  Eigen::Vector3f Pc = mRcw * P + mTcw.translation();
  float &PcX = Pc(0);
  float &PcY = Pc(1);
  float &PcZ = Pc(2);

  // Check positive depth
  if (PcZ < 0.0f) {
    cout << "Negative depth: " << PcZ << endl;
    return false;
  }

  // Project in image and check it is not outside
  float invz = 1.0f / PcZ;
  u = fx * PcX * invz + cx;
  v = fy * PcY * invz + cy;

  // cout << "c";

  if (u < mnMinX || u > mnMaxX) return false;
  if (v < mnMinY || v > mnMaxY) return false;

  float x = (u - cx) * invfx;
  float y = (v - cy) * invfy;
  float r2 = x * x + y * y;
  float k1 = mDistCoef.at<float>(0);
  float k2 = mDistCoef.at<float>(1);
  float p1 = mDistCoef.at<float>(2);
  float p2 = mDistCoef.at<float>(3);
  float k3 = 0;
  if (mDistCoef.total() == 5) {
    k3 = mDistCoef.at<float>(4);
  }

  // Radial distorsion
  float x_distort = x * (1 + k1 * r2 + k2 * r2 * r2 + k3 * r2 * r2 * r2);
  float y_distort = y * (1 + k1 * r2 + k2 * r2 * r2 + k3 * r2 * r2 * r2);

  // Tangential distorsion
  x_distort = x_distort + (2 * p1 * x * y + p2 * (r2 + 2 * x * x));
  y_distort = y_distort + (p1 * (r2 + 2 * y * y) + 2 * p2 * x * y);

  float u_distort = x_distort * fx + cx;
  float v_distort = y_distort * fy + cy;

  u = u_distort;
  v = v_distort;

  kp = cv::Point2f(u, v);

  return true;
}

bool KeyFrame::ProjectPointUnDistort(MapPoint *pMP, cv::Point2f &kp, float &u,
                                     float &v) {
  // 3D in absolute coordinates
  Eigen::Vector3f P = pMP->GetWorldPos();

  // 3D in camera coordinates
  Eigen::Vector3f Pc = mRcw * P + mTcw.translation();
  float &PcX = Pc(0);
  float &PcY = Pc(1);
  float &PcZ = Pc(2);

  // Check positive depth
  if (PcZ < 0.0f) {
    cout << "Negative depth: " << PcZ << endl;
    return false;
  }

  // Project in image and check it is not outside
  const float invz = 1.0f / PcZ;
  u = fx * PcX * invz + cx;
  v = fy * PcY * invz + cy;

  if (u < mnMinX || u > mnMaxX) return false;
  if (v < mnMinY || v > mnMaxY) return false;

  kp = cv::Point2f(u, v);

  return true;
}

Sophus::SE3f KeyFrame::GetRelativePoseTrl() {
  unique_lock<mutex> lock(mMutexPose);
  return mTrl;
}

Sophus::SE3f KeyFrame::GetRelativePoseTlr() {
  unique_lock<mutex> lock(mMutexPose);
  return mTlr;
}

Sophus::SE3<float> KeyFrame::GetRightPose() {
  unique_lock<mutex> lock(mMutexPose);

  return mTrl * mTcw;
}

Sophus::SE3<float> KeyFrame::GetRightPoseInverse() {
  unique_lock<mutex> lock(mMutexPose);

  return mTwc * mTlr;
}

Eigen::Vector3f KeyFrame::GetRightCameraCenter() {
  unique_lock<mutex> lock(mMutexPose);

  return (mTwc * mTlr).translation();
}

Eigen::Matrix<float, 3, 3> KeyFrame::GetRightRotation() {
  unique_lock<mutex> lock(mMutexPose);

  return (mTrl.so3() * mTcw.so3()).matrix();
}

Eigen::Vector3f KeyFrame::GetRightTranslation() {
  unique_lock<mutex> lock(mMutexPose);
  return (mTrl * mTcw).translation();
}

void KeyFrame::SetORBVocabulary(ORBVocabulary *pORBVoc) {
  mpORBvocabulary = pORBVoc;
}

void KeyFrame::SetKeyFrameDatabase(KeyFrameDatabase *pKFDB) {
  mpKeyFrameDB = pKFDB;
}

// Assign the static flag to keypoint according to the depth image.
// If there is depth value, the keypoint is static. If there is no depth value, the keypoint is dynamic.
void KeyFrame::selectStaticFeatures()
{
    int count_dyn_pts = 0;
    static_flags_.clear();
    float cam_dmin = 0.01;
    float cam_dmax = 20.0;
    for ( size_t nf = 0; nf < mvKeysUn.size(); nf ++ ) {

        const cv::Point2f& kpt = mvKeysUn.at ( nf ).pt;
        const float& dp = depth_.at<float> ( kpt.y, kpt.x );

        if ( dp > 0 ) {
            static_flags_.push_back ( true );
        } else if(dp < 0.0f) {
            static_flags_.push_back ( false );
            mvpMapPoints.at(nf) = static_cast<MapPoint*>(NULL); // delete the mpt.
           
            mvDynKeysUn.push_back(mvKeysUn[nf]);
            
            count_dyn_pts++;
        }
    } // for all keypoints.
    cout << "Deleted " << count_dyn_pts << " MPs by unmatched Depth threshold " << endl;
} // selectStaticFeatures

}  // namespace ORB_SLAM3
