#pragma once

#include <vtkCellArray.h>
#include <vtkDoubleArray.h>
#include <vtkLine.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkStringArray.h>
#include <vtkTriangle.h>
#include <vtkUnsignedIntArray.h>
#include <vtkXMLPolyDataWriter.h>

// #include "rti/geo/i_boundary.hpp"
// #include "rti/geo/point_cloud_sphere_geometry.hpp"
// #include "rti/geo/point_cloud_disc_geometry.hpp"
// #include "rti/geo/triangle_geometry.hpp"
// #include "rti/trace/i_hit_accumulator.hpp"

namespace io {

  template<typename TT>
  using pair = std::array<TT, 2>;

  template<typename TT>
  using triple = std::array<TT, 3>;

  template<typename Ty>
  class vtp_writer {
  public:

    static void
    write(std::vector<std::array<Ty, 3> > points,
          std::vector<std::array<Ty, 3> > normals,
          std::vector<Ty> radii,
          std::vector<Ty> mcestimates,
          std::vector<size_t> hitcnts,
          std::string outfilename)
    {
      auto polydata = create_polydata();
      add_points(polydata, points);
      add_normals(polydata, normals);
      add_radii(polydata, radii);
      add_mcestimates(polydata, mcestimates);
      add_hit_cnts(polydata, hitcnts);
      write(polydata, outfilename);
    }

    // static
    // void write(rti::geo::absc_point_cloud_geometry<Ty>& pGeometry,
    //            rti::trace::i_hit_accumulator<Ty>& pHA,
    //            std::string pOutfilename,
    //            std::vector<pair<std::string> >& pMetadata) {
    //   // Precondition:
    //   assert (pGeometry.get_num_primitives() == pHA.get_values().size() &&
    //           "hit count accumulator does not fit the given geometry");
    //   auto polydata = get_polydata(pGeometry);
    //   add_hit_values_to_points(polydata, pHA);
    //   try_add_hit_counts_to_points(polydata, pHA);
    //   //try_add_statistical_data_to_points(polydata, pHA);
    //   try_add_statistical_data(polydata, pHA);
    //   add_metadata(polydata, pMetadata);
    //   write(polydata, pOutfilename);
    // }

    // static
    // void write(rti::geo::triangle_geometry<Ty>& pGeometry,
    //            rti::trace::i_hit_accumulator<Ty>& pHA,
    //            std::string pOutfilename,
    //            std::vector<pair<std::string> >& pMetadata) {
    //   // Precondition:
    //   assert (pGeometry.get_num_primitives() == pHA.get_values().size() &&
    //           "hit count accumulator does not fit the given geometry");
    //   auto polydata = get_polydata(pGeometry);
    //   add_hit_values_to_triangles(polydata, pHA);
    //   try_add_hit_counts_to_triangles(polydata, pHA);
    //   //try_add_statistical_data_to_triangles(polydata, pHA);
    //   try_add_statistical_data(polydata, pHA);
    //   add_metadata(polydata, pMetadata);
    //   write(polydata, pOutfilename);
    // }

    // static
    // void write(rti::geo::i_boundary<Ty>& pBoundary,
    //            std::string pOutfilename) {
    //   auto polydata = get_polydata(pBoundary);
    //   write(polydata, pOutfilename);
    // }

    static
    void write(std::vector<pair<triple<Ty> > >* pVec,
               std::string pOutfilename) {
      auto polydata = get_polydata(*pVec);
      write(polydata, pOutfilename);
    }

    static
    void write(std::vector<triple<Ty> >* pVec,
               std::string pOutfilename) {
      auto polydata = get_polydata(*pVec);
      write(polydata, pOutfilename);
    }

  private:

    static
    void write(vtkSmartPointer<vtkPolyData> pPolydata, std::string pOutfilename) {
      auto writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
      writer->SetFileName(pOutfilename.c_str());
      writer->SetInputData(pPolydata);
      writer->SetDataModeToAscii(); // human readable XML output
      writer->Write();
    }

    static
    void add_metadata(vtkSmartPointer<vtkPolyData> pPolydata,
                      std::vector<pair<std::string> >& pMetadata) {
      for (auto& pair : pMetadata) {
        auto va = vtkSmartPointer<vtkStringArray>::New();
        va->SetNumberOfComponents(1); // one string per entry
        va->SetName(pair[0].c_str());
        va->InsertNextValue(pair[1].c_str());
        pPolydata->GetFieldData()->AddArray(va);
      }
    }

    // static
    // void add_hit_values_to_points(vtkSmartPointer<vtkPolyData> pPolydata,
    //                     rti::trace::i_hit_accumulator<Ty>& pAc) {
    //   auto invalues = pAc.get_values();
    //   assert (pPolydata->GetNumberOfPoints() == invalues.size() &&
    //           "polydata does not fit the hit count accumulator");
    //   auto hitValues = vtkSmartPointer<vtkDoubleArray>::New();
    //   hitValues->SetNumberOfComponents(1); // 1 dimension
    //   hitValues->SetNumberOfTuples(invalues.size());
    //   for (size_t idx = 0; idx < invalues.size(); ++idx) {
    //     //if (invalues[idx] != 0) RLOG_DEBUG << "writing " << invalues[idx] << " to vtkPolyData" << std::endl;
    //     hitValues->InsertValue(idx, invalues[idx]);
    //   }
    //   hitValues->SetName(valueStr);
    //   pPolydata->GetCellData()->AddArray(hitValues);
    // }

    // // TODO: Combine with add_hit_values_to_points into one abstraction?
    // static
    // void add_hit_values_to_triangles(vtkSmartPointer<vtkPolyData> pPolydata,
    //                                  rti::trace::i_hit_accumulator<Ty>& pAc) {
    //   auto invalues = pAc.get_values();
    //   assert (pPolydata->GetNumberOfCells() == invalues.size() &&
    //           "number of cells in polydata does not fit the hit counter accumulator");
    //   auto hitValues = vtkSmartPointer<vtkDoubleArray>::New();
    //   hitValues->SetNumberOfComponents(1); // 1 dimension
    //   hitValues->SetNumberOfTuples(invalues.size());
    //   for (size_t idx = 0; idx < invalues.size(); ++idx) {
    //     hitValues->InsertValue(idx, invalues[idx]);
    //   }
    //   hitValues->SetName(valueStr);
    //   pPolydata->GetCellData()->AddArray(hitValues);
    // }

    // static
    // void try_add_hit_counts_to_points(vtkSmartPointer<vtkPolyData> pPolydata,
    //                         rti::trace::i_hit_accumulator<Ty>& pAc) {
    //   auto incnts = pAc.get_cnts();
    //   if (incnts.size() <= 0)
    //     return; // no hit counts; simply return
    //   auto hitCnts = vtkSmartPointer<vtkUnsignedIntArray>::New();
    //   hitCnts->SetNumberOfComponents(1); // 1 dimension
    //   hitCnts->SetNumberOfTuples(incnts.size());
    //   for (size_t idx = 0; idx < incnts.size(); ++idx) {
    //     hitCnts->InsertValue(idx, incnts[idx]);
    //   }
    //   hitCnts->SetName(hitcntStr);
    //   pPolydata->GetCellData()->AddArray(hitCnts);
    // }

    // static
    // void try_add_hit_counts_to_triangles(vtkSmartPointer<vtkPolyData> pPolydata,
    //                                  rti::trace::i_hit_accumulator<Ty>& pAc) {
    //   auto incnts = pAc.get_cnts();
    //   assert (pPolydata->GetNumberOfCells() == incnts.size() &&
    //           "number of cells in polydata does not fit the hit counter accumulator");
    //   if (incnts.size() <= 0)
    //     return; // no hit counts; simply return
    //   auto hitCnts = vtkSmartPointer<vtkUnsignedIntArray>::New();
    //   hitCnts->SetNumberOfComponents(1); // 1 dimension
    //   hitCnts->SetNumberOfTuples(incnts.size());
    //   for (size_t idx = 0; idx < incnts.size(); ++idx) {
    //     hitCnts->InsertValue(idx, incnts[idx]);
    //   }
    //   hitCnts->SetName(hitcntStr);
    //   pPolydata->GetCellData()->AddArray(hitCnts);
    // }

    // static
    // void try_add_statistical_data_to_points(vtkSmartPointer<vtkPolyData> pPolydata,
    //                               rti::trace::i_hit_accumulator<Ty>& pAc) {
    //   auto indatarerr = pAc.get_relative_error();
    //   if (indatarerr.size() <= 0)
    //     return; // no data; just return
    //   auto rerr = vtkSmartPointer<vtkDoubleArray>::New();
    //   rerr->SetNumberOfComponents(1); // 1 dimension
    //   rerr->SetNumberOfTuples(indatarerr.size());
    //   for (size_t idx = 0; idx < indatarerr.size(); ++idx) {
    //     rerr->InsertValue(idx, indatarerr[idx]);
    //   }
    //   rerr->SetName(relativeErrorStr);
    //   pPolydata->GetCellData()->AddArray(rerr);
    //   //
    //   auto indatavov = pAc.get_vov();
    //   if (indatavov.size() <= 0)
    //     return;
    //   auto vov = vtkSmartPointer<vtkDoubleArray>::New();
    //   vov->SetNumberOfComponents(1); // 1 dimension
    //   vov->SetNumberOfTuples(indatavov.size());
    //   for (size_t idx = 0; idx < indatavov.size(); ++idx) {
    //     vov->InsertValue(idx, indatavov[idx]);
    //   }
    //   vov->SetName(varOfVarStr);
    //   pPolydata->GetCellData()->AddArray(vov);
    // }

    // static
    // void try_add_statistical_data_to_triangles(vtkSmartPointer<vtkPolyData> pPolydata,
    //                                      rti::trace::i_hit_accumulator<Ty>& pAc) {
    //   auto indata = pAc.get_relative_error();
    //   assert (pPolydata->GetNumberOfCells() == indata.size() &&
    //           "number of cells in polydata does not fit the hit counter accumulator");
    //   if (indata.size() <= 0)
    //     return; // no data; just return
    //   auto rerr = vtkSmartPointer<vtkDoubleArray>::New();
    //   rerr->SetNumberOfComponents(1); // 1 dimension
    //   rerr->SetNumberOfTuples(indata.size());
    //   for (size_t idx = 0; idx < indata.size(); ++idx) {
    //     rerr->InsertValue(idx, indata[idx]);
    //   }
    //   rerr->SetName(relativeErrorStr);
    //   pPolydata->GetCellData()->AddArray(rerr);
    // }

    // static
    // void try_add_statistical_data(vtkSmartPointer<vtkPolyData> pPolydata,
    //                               rti::trace::i_hit_accumulator<Ty>& pAc) {
    //   auto indatarerr = pAc.get_relative_error();
    //   assert (pPolydata->GetNumberOfCells() == indatarerr.size() &&
    //           "number of cells in polydata does not fit the accumulator");
    //   if (indatarerr.size() <= 0)
    //     return; // no data; just return
    //   auto rerr = vtkSmartPointer<vtkDoubleArray>::New();
    //   rerr->SetNumberOfComponents(1); // 1 dimension
    //   rerr->SetNumberOfTuples(indatarerr.size());
    //   for (size_t idx = 0; idx < indatarerr.size(); ++idx) {
    //     rerr->InsertValue(idx, indatarerr[idx]);
    //   }
    //   rerr->SetName(relativeErrorStr);
    //   pPolydata->GetCellData()->AddArray(rerr);
    //   //
    //   auto indatavov = pAc.get_vov();
    //   assert (pPolydata->GetNumberOfCells() == indatavov.size() &&
    //           "number of cells in polydata does not fit the accumulator");
    //   if (indatavov.size() <= 0)
    //     return;
    //   auto vov = vtkSmartPointer<vtkDoubleArray>::New();
    //   vov->SetNumberOfComponents(1); // 1 dimension
    //   vov->SetNumberOfTuples(indatavov.size());
    //   for (size_t idx = 0; idx < indatavov.size(); ++idx) {
    //     vov->InsertValue(idx, indatavov[idx]);
    //   }
    //   vov->SetName(varOfVarStr);
    //   pPolydata->GetCellData()->AddArray(vov);
    // }

    // static
    // vtkSmartPointer<vtkPolyData> get_polydata(rti::geo::triangle_geometry<Ty>& pGeometry) {
    //   auto inpoints = pGeometry.get_vertices();
    //   auto intriangles = pGeometry.get_triangles();
    //   auto numpoints = inpoints.size();
    //   auto numtriangles = intriangles.size();

    //   auto vtkpoints = vtkSmartPointer<vtkPoints>::New();
    //   auto vtkcells = vtkSmartPointer<vtkCellArray>::New();

    //   // Handle points
    //   for (size_t idx = 0; idx < numpoints; ++idx) {
    //     auto& point = inpoints[idx];
    //     auto writepointID = vtkpoints->InsertNextPoint(point[0], point[1], point[2]);
    //   }
    //   // Handle triangles
    //   for (size_t idx = 0; idx < numtriangles; ++idx) {
    //     auto& intriangle = intriangles[idx];
    //     auto outtriangle = vtkSmartPointer<vtkTriangle>::New();
    //     outtriangle->GetPointIds()->SetId (0, intriangle[0]);
    //     outtriangle->GetPointIds()->SetId (1, intriangle[1]);
    //     outtriangle->GetPointIds()->SetId (2, intriangle[2]);
    //     vtkcells->InsertNextCell(outtriangle);
    //   }
    //   auto polydata = vtkSmartPointer<vtkPolyData>::New();
    //   polydata->SetPoints(vtkpoints);
    //   polydata->SetPolys(vtkcells);
    //   return polydata;
    // }

    // static
    // vtkSmartPointer<vtkPolyData> get_polydata(rti::geo::absc_point_cloud_geometry<Ty>& pGeometry) {
    //   auto points = vtkSmartPointer<vtkPoints>::New();
    //   auto cells = vtkSmartPointer<vtkCellArray>::New();

    //   auto numpoints = pGeometry.get_num_primitives();
    //   //auto normals = std::vector<triple<Ty> > {numpoints};
    //   auto normals = vtkSmartPointer<vtkDoubleArray>::New();
    //   normals->SetNumberOfComponents(3); // 3 dimensions
    //   normals->SetNumberOfTuples(numpoints);
    //   //auto radii = std::vector<Ty> {numpoints};
    //   auto radii = vtkSmartPointer<vtkDoubleArray>::New();
    //   radii->SetNumberOfComponents(1); // 1 dimensions
    //   radii->SetNumberOfTuples(numpoints);
    //   for (size_t idx = 0; idx < numpoints; ++idx) {
    //     auto point = pGeometry.get_prim(idx);
    //     auto normal = pGeometry.get_normal(idx);
    //     auto writePointId = points->InsertNextPoint(point[0], point[1], point[2]);
    //     cells->InsertNextCell(1, &writePointId); // one cell for writePointId
    //     //normals.push_back(normal);
    //     normals->SetTuple(idx, normal.data());
    //     //radii.push_back(point[3]);
    //     // the radius is saved as the 4th element of the point (which is an array)
    //     radii->SetTuple(idx, &point[3]);
    //   }
    //   auto polydata = vtkSmartPointer<vtkPolyData>::New();
    //   polydata->SetPoints(points);
    //   polydata->SetVerts(cells);
    //   polydata->GetCellData()->SetNormals(normals);
    //   radii->SetName(radiusStr);
    //   polydata->GetCellData()->AddArray(radii);

    //   return polydata;
    // }

    // static
    // vtkSmartPointer<vtkPolyData> get_polydata(rti::geo::i_boundary<Ty>& pBoundary) {
    //   auto pointsOut = vtkSmartPointer<vtkPoints>::New();
    //   auto trianglesOut = vtkSmartPointer<vtkCellArray>::New();
    //   auto normalsOut = vtkSmartPointer<vtkDoubleArray>::New();
    //   for (auto const& pin : pBoundary.get_vertices()) {
    //     pointsOut->InsertNextPoint(pin[0], pin[1], pin[2]);
    //   }
    //   for (auto const& tin : pBoundary.get_triangles()) {
    //     auto triangleOut = vtkSmartPointer<vtkTriangle>::New();
    //     triangleOut->GetPointIds()->SetId(0, tin[0]);
    //     triangleOut->GetPointIds()->SetId(1, tin[1]);
    //     triangleOut->GetPointIds()->SetId(2, tin[2]);
    //     trianglesOut->InsertNextCell(triangleOut);
    //   }
    //   normalsOut->SetNumberOfComponents(3); // 3 dimensions
    //   normalsOut->SetNumberOfTuples(trianglesOut->GetNumberOfCells());
    //   // for (auto const& nin : pBoundary.get_triangle_normals()) {
    //   //   normalsOut->InsertNextTuple(nin.data()); // 0 is the start index of the second argument
    //   // }
    //   auto nin = pBoundary.get_triangle_normals();
    //   for (size_t idx = 0; idx < nin.size(); ++idx) {
    //     normalsOut->SetTuple(idx, nin[idx].data());
    //   }

    //   auto polydata = vtkSmartPointer<vtkPolyData>::New();
    //   polydata->SetPoints(pointsOut);
    //   polydata->SetPolys(trianglesOut);
    //   polydata->GetCellData()->SetNormals(normalsOut);
    //   return polydata;
    // }

    static vtkSmartPointer<vtkPolyData>
    create_polydata()
    {
      return vtkSmartPointer<vtkPolyData>::New();
    }

    static void
    add_points(vtkSmartPointer<vtkPolyData> polydata, std::vector<std::array<Ty, 3> > points)
    {
      auto pointsOut = vtkSmartPointer<vtkPoints>::New();
      auto cells = vtkSmartPointer<vtkCellArray>::New();
      for (auto const& point : points) {
        // pointsOut->InsertNextPoint(point.data());
        auto outPointId = pointsOut->InsertNextPoint(point[0], point[1], point[2]);
        cells->InsertNextCell(1, &outPointId);
      }
      polydata->SetPoints(pointsOut);
      polydata->SetVerts(cells);
    }

    static void
    add_normals(vtkSmartPointer<vtkPolyData> polydata, std::vector<std::array<Ty, 3> > normals)
    {
      auto normalsOut = vtkSmartPointer<vtkDoubleArray>::New();
      normalsOut->SetNumberOfComponents(3); // 3 dimensions
      normalsOut->SetNumberOfTuples(normals.size());
      auto idx = 0u;
      for (auto const& nn : normals) {
        normalsOut->SetTuple(idx, nn.data());
        idx += 1;
      }
      polydata->GetCellData()->SetNormals(normalsOut);
    }

    static void
    add_radii(vtkSmartPointer<vtkPolyData> polydata, std::vector<Ty> radii)
    {
      auto radiiOut = vtkSmartPointer<vtkDoubleArray>::New();
      radiiOut->SetNumberOfComponents(1); // 1 dimension
      radiiOut->SetNumberOfTuples(radii.size());
      auto idx = 0u;
      for (auto const& rr: radii) {
        radiiOut->SetTuple(idx, &rr);
        idx += 1;
      }
      radiiOut->SetName(radiusStr);
      polydata->GetCellData()->AddArray(radiiOut);
    }

    static void
    add_mcestimates(vtkSmartPointer<vtkPolyData> polydata, std::vector<Ty> mcestimates)
    {
      auto mcOut = vtkSmartPointer<vtkDoubleArray>::New();
      mcOut->SetNumberOfComponents(1); // 1 dimension
      mcOut->SetNumberOfTuples(mcestimates.size());
      auto idx = 0u;
      for (auto const& mc : mcestimates) {
        mcOut->SetTuple(idx, &mc);
        idx += 1;
      }
      mcOut->SetName(mcestimateStr);
      polydata->GetCellData()->AddArray(mcOut);
    }

    static void
    add_hit_cnts(vtkSmartPointer<vtkPolyData> polydata, std::vector<size_t> hitcnts)
    {
      auto hitcntsOut = vtkSmartPointer<vtkUnsignedIntArray>::New();
      hitcntsOut->SetNumberOfComponents(1); // 1 dimension
      hitcntsOut->SetNumberOfTuples(hitcnts.size());
      auto idx = 0;
      for (auto const& hitcnt : hitcnts) {
        hitcntsOut->InsertValue(idx, hitcnt);
        idx += 1;
      }
      hitcntsOut->SetName(hitcntStr);
      polydata->GetCellData()->AddArray(hitcntsOut);
    }

    static
    vtkSmartPointer<vtkPolyData> get_polydata(
        std::vector<pair<triple<float> > >& pVec) {
      auto pointsOut = vtkSmartPointer<vtkPoints>::New();
      auto linesOut = vtkSmartPointer<vtkCellArray>::New();

      auto pidx = 0ull;
      for (auto const& lineIn : pVec) {
        pointsOut->InsertNextPoint(lineIn[0].data());
        pointsOut->InsertNextPoint(lineIn[1].data());
        auto lineOut = vtkSmartPointer<vtkLine>::New();
        lineOut->GetPointIds()->SetId(0,pidx+0);
        lineOut->GetPointIds()->SetId(1,pidx+1);
        linesOut->InsertNextCell(lineOut);
        pidx += 2; // 2 points have been inserted
      }
      assert (pidx == 2 * pVec.size() && "Missmatch between point-index and size of vector");

      auto polydata = vtkSmartPointer<vtkPolyData>::New();
      polydata->SetPoints(pointsOut);
      polydata->SetLines(linesOut);
      return polydata;
    }

    static
    vtkSmartPointer<vtkPolyData> get_polydata(
        std::vector<triple<float> >& pVec) {
      auto pointsOut =vtkSmartPointer<vtkPoints>::New();
      for (auto const& pointIn : pVec) {
        pointsOut->InsertNextPoint(pointIn.data());
      }
      auto polydata = vtkSmartPointer<vtkPolyData>::New();
      polydata->SetPoints(pointsOut);
      return polydata;
    }

    static constexpr char const* radiusStr = "radius";
    static constexpr char const* valueStr = "value";
    static constexpr char const* mcestimateStr = "mc-estimate";
    static constexpr char const* hitcntStr = "hitcnt";
    static constexpr char const* relativeErrorStr = "relative-error";
    static constexpr char const* varOfVarStr = "variance-of-variance";
  };
}
